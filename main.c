#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// aturan sistem (mendefinisikan konstanta)
#define MAX_STUDENTS 25
#define MAX_NAME_LENGTH 50
#define MAX_SUBJECTS 8

// Struct untuk menyimpan data mahasiswa
typedef struct {
    int NIM;
    char name[MAX_NAME_LENGTH];
    float grades[MAX_SUBJECTS]; // Nilai untuk mata kuliah
} Student;

// Nama mata kuliah 
char subjects[MAX_SUBJECTS][MAX_NAME_LENGTH];
char subjectCodes[MAX_SUBJECTS][10];

// Fungsi untuk menyimpan data ke file
void saveDataToFile(Student students[], int count) {
    FILE *file = fopen("students_data.txt", "w");
    if (!file) {
        printf("\nGagal membuka file untuk menyimpan data.\n");
        return;
    }

    fprintf(file, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n%s\n", students[i].NIM, students[i].name);
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            fprintf(file, "%.2f ", students[i].grades[j]);
        }
        fprintf(file, "\n");
    }

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        fprintf(file, "%s\n%s\n", subjects[i], subjectCodes[i]);
    }

    fclose(file);
}

// Fungsi untuk memuat data dari file
int loadDataFromFile(Student students[]) {
    FILE *file = fopen("students_data.txt", "r");
    if (!file) {
        printf("\nFile data tidak ditemukan. Memulai dengan data kosong.\n");
        return 0;
    }

    int count;
    fscanf(file, "%d\n", &count);
    for (int i = 0; i < count; i++) {
        fscanf(file, "%d\n", &students[i].NIM);
        fgets(students[i].name, MAX_NAME_LENGTH, file);
        students[i].name[strcspn(students[i].name, "\n")] = 0; // Menghapus newline
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            fscanf(file, "%f", &students[i].grades[j]);
        }
        fscanf(file, "\n");
    }

    for (int i = 0; i < MAX_SUBJECTS; i++) {
        fgets(subjects[i], MAX_NAME_LENGTH, file);
        subjects[i][strcspn(subjects[i], "\n")] = 0; // Menghapus newline
        fgets(subjectCodes[i], 10, file);
        subjectCodes[i][strcspn(subjectCodes[i], "\n")] = 0; // Menghapus newline
    }

    fclose(file);
    printf("\nData berhasil dimuat dari file.\n");
    return count;
}

// Fungsi untuk menginput nama mata kuliah
void inputSubjects() {
    printf("\nMasukkan Nama dan Kode Mata Kuliah :\n");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("  Nama Mata Kuliah ke-%d: ", i + 1);
        fgets(subjects[i], MAX_NAME_LENGTH, stdin);
        subjects[i][strcspn(subjects[i], "\n")] = 0; // Menghapus newline

        printf("  Kode Mata Kuliah ke-%d: ", i + 1);
        fgets(subjectCodes[i], 10, stdin);
        subjectCodes[i][strcspn(subjectCodes[i], "\n")] = 0; // Menghapus newline
    }
    printf("\nNama dan kode mata kuliah berhasil disimpan!\n");
}

// Fungsi untuk menambahkan mahasiswa
int addStudent(Student students[], int count) {
    if (count >= MAX_STUDENTS) {
        printf("\nMaaf, Sistem Sudah Mencapai Kapasitas Maksimal Penambahan Mahasiswa!\n");
        return count;
    }

    printf("\nMasukkan Nomor Induk Mahasiswa(NIM): ");
    scanf("%d", &students[count].NIM);
    getchar(); // Membersihkan newline dari buffer
    printf("Masukkan Nama Mahasiswa: ");
    fgets(students[count].name, MAX_NAME_LENGTH, stdin);
    students[count].name[strcspn(students[count].name, "\n")] = 0; // Menghapus newline

    printf("\nMasukkan Nilai untuk Setiap Mata Kuliah:\n");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("  Nilai untuk %s (%s): ", subjects[i], subjectCodes[i]);
        scanf("%f", &students[count].grades[i]);
    }

    printf("\nData Mahasiswa berhasil ditambahkan!\n");
    saveDataToFile(students, count + 1); // Autosave setelah penambahan data
    return count + 1;
}

// Fungsi untuk menampilkan data mahasiswa
void displayStudents(Student students[], int count) {
    if (count == 0) {
        printf("\nBelum ada data mahasiswa untuk ditampilkan.\n");
        return;
    }

    // Hitung panjang maksimal untuk header dan isi data setiap kolom
    int maxNameLength = strlen("Nama Mahasiswa");
    int maxSubjectLength[MAX_SUBJECTS] = {0};

    for (int i = 0; i < count; i++) {
        if ((int)strlen(students[i].name) > maxNameLength) {
            maxNameLength = strlen(students[i].name);
        }
    }

    for (int j = 0; j < MAX_SUBJECTS; j++) {
        maxSubjectLength[j] = strlen(subjectCodes[j]);
        for (int i = 0; i < count; i++) {
            char gradeBuffer[10];
            snprintf(gradeBuffer, 10, "%.2f", students[i].grades[j]);
            if ((int)strlen(gradeBuffer) > maxSubjectLength[j]) {
                maxSubjectLength[j] = strlen(gradeBuffer);
            }
        }
    }

    // Tampilkan header tabel
    printf("\n==========================================================================================================================\n");
    printf("| %-3s | %-10s | %-*s |", "ID", "NIM", maxNameLength, "Nama Mahasiswa");
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf(" %-*s |", maxSubjectLength[i], subjectCodes[i]);
    }
    printf("\n==========================================================================================================================\n");

    // Tampilkan data mahasiswa
    for (int i = 0; i < count; i++) {
        printf("| %-3d | %-10d | %-*s |", i + 1, students[i].NIM, maxNameLength, students[i].name);
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            printf(" %-*.2f |", maxSubjectLength[j], students[i].grades[j]);
        }
        printf("\n");
    }

    printf("==========================================================================================================================\n");
}

// Fungsi untuk mencari mahasiswa berdasarkan NIM
void searchStudentByNIM(Student students[], int count) {
    if (count == 0) {
        printf("\nBelum ada data mahasiswa untuk dicari.\n");
        return;
    }

    int searchNIM;
    printf("\nMasukkan NIM yang ingin dicari: ");
    scanf("%d", &searchNIM);

    for (int i = 0; i < count; i++) {
        if (students[i].NIM == searchNIM) {
            // Hitung panjang maksimal untuk header dan isi data setiap kolom
            int maxNameLength = strlen("Nama Mahasiswa");
            int maxSubjectLength[MAX_SUBJECTS] = {0};

            if ((int)strlen(students[i].name) > maxNameLength) {
                maxNameLength = strlen(students[i].name);
            }

            for (int j = 0; j < MAX_SUBJECTS; j++) {
                maxSubjectLength[j] = strlen(subjectCodes[j]);
                char gradeBuffer[10];
                snprintf(gradeBuffer, 10, "%.2f", students[i].grades[j]);
                if ((int)strlen(gradeBuffer) > maxSubjectLength[j]) {
                    maxSubjectLength[j] = strlen(gradeBuffer);
                }
            }

            // Tampilkan header tabel
            printf("\n===============================================================================================================\n");
            printf("| %-10s | %-*s |", "NIM", maxNameLength, "Nama Mahasiswa");
            for (int j = 0; j < MAX_SUBJECTS; j++) {
                printf(" %-*s |", maxSubjectLength[j], subjectCodes[j]);
            }
            printf("\n===============================================================================================================\n");

            // Tampilkan data mahasiswa
            printf("| %-10d | %-*s |", students[i].NIM, maxNameLength, students[i].name);
            for (int j = 0; j < MAX_SUBJECTS; j++) {
                printf(" %-*.2f |", maxSubjectLength[j], students[i].grades[j]);
            }
            printf("\n===============================================================================================================\n");

            return;
        }
    }

    printf("\nMahasiswa dengan NIM %d tidak ditemukan.\n", searchNIM);
}


int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0;
    int choice;
    int subjectsInitialized = 0;

    studentCount = loadDataFromFile(students);
    subjectsInitialized = studentCount > 0;

    do {
        printf("\nSistem Manajemen Data Mahasiswa Program Studi Teknik Informatika Kelas 1B\n");
        printf("1. Input Nama & Kode Mata Kuliah\n");
        printf("2. Tambah Data Mahasiswa\n");
        printf("3. Tampilkan Semua Data Mahasiswa\n");
        printf("4. Cari Data Mahasiswa Berdasarkan NIM\n");
        printf("5. Keluar\n");
        printf("\nSilahkan Masukkan Opsi Pilihan: ");
        scanf("%d", &choice);
        getchar(); // Membersihkan newline dari buffer


        switch (choice) {
            case 1:
                inputSubjects();
                subjectsInitialized = 1;
                break;
            case 2:
                if (!subjectsInitialized) {
                    printf("\nSilakan input nama dan kode mata kuliah terlebih dahulu!\n");
                } else {
                    studentCount = addStudent(students, studentCount);
                }
                break;
            case 3:
                displayStudents(students, studentCount);
                break;
            case 4:
                searchStudentByNIM(students, studentCount);
                break;

            case 5:
                saveDataToFile(students, studentCount);
                printf("\nKeluar dari program.\n");
                break;
            default:
                printf("\nPilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (choice != 5);

    return 0;
}
