
#include <stdio.h>
#include <string.h>

#define MAX 500
#define SUBJECTS 10
#define PAGE_SIZE 10

// Student structure
struct Student
{
    int roll;
    char name[50];
    char studentClass[20];
    int marks[SUBJECTS];  // -1 means N/A
};

// Global variables
struct Student students[MAX];
int studentCount = 0;
int bestSubjects = 5;

// Subject names
char subjectNames[SUBJECTS][20] = {
    "English", "Mathematics", "Physics", "Chemistry", "Computer",
    "Biology", "History", "Geography", "Hindi", "PhysicalEd"
};

// Find student using roll number
int findStudent(int roll)
{
    int i;

    for (i = 0; i < studentCount; i++)
    {
        if (students[i].roll == roll)
        {
            return i;
        }
    }

    return -1;
}

// Save students in a text file
void saveData()
{
    FILE *fp = fopen("students.txt", "w");
    int i, j;

    if (fp == NULL)
    {
        printf("Unable to save data!\n");
        return;
    }

    fprintf(fp, "%d %d\n", studentCount, bestSubjects);

    for (i = 0; i < studentCount; i++)
    {
        fprintf(fp, "%d\n", students[i].roll);
        fprintf(fp, "%s\n", students[i].name);
        fprintf(fp, "%s\n", students[i].studentClass);

        for (j = 0; j < SUBJECTS; j++)
        {
            fprintf(fp, "%d ", students[i].marks[j]);
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}

// Load saved students
void loadData()
{
    FILE *fp = fopen("students.txt", "r");
    int i, j;

    if (fp == NULL)
    {
        return;
    }

    if (fscanf(fp, "%d %d", &studentCount, &bestSubjects) != 2)
    {
        studentCount = 0;
        bestSubjects = 5;
        fclose(fp);
        return;
    }

    if (studentCount < 0 || studentCount > MAX)
    {
        studentCount = 0;
        bestSubjects = 5;
        fclose(fp);
        return;
    }

    if (bestSubjects < 1 || bestSubjects > SUBJECTS)
    {
        bestSubjects = 5;
    }

    for (i = 0; i < studentCount; i++)
    {
        if (fscanf(fp, "%d", &students[i].roll) != 1)
        {
            studentCount = i;
            break;
        }

        fgetc(fp);

        if (fgets(students[i].name, 50, fp) == NULL ||
            fgets(students[i].studentClass, 20, fp) == NULL)
        {
            studentCount = i;
            break;
        }

        students[i].name[strcspn(students[i].name, "\n")] = '\0';
        students[i].studentClass[
            strcspn(students[i].studentClass, "\n")
        ] = '\0';

        for (j = 0; j < SUBJECTS; j++)
        {
            if (fscanf(fp, "%d", &students[i].marks[j]) != 1)
            {
                students[i].marks[j] = -1;
            }
        }
    }

    fclose(fp);
}

// Calculate percentage using best applicable marks
void showResults(int index)
{
    int marks[SUBJECTS];
    int i, j, temp;
    int total = 0;
    int subjectsTaken = 0;
    float percentage;

    for (i = 0; i < SUBJECTS; i++)
    {
        marks[i] = students[index].marks[i];
    }

    // Sort marks from highest to lowest
    for (i = 0; i < SUBJECTS - 1; i++)
    {
        for (j = i + 1; j < SUBJECTS; j++)
        {
            if (marks[i] < marks[j])
            {
                temp = marks[i];
                marks[i] = marks[j];
                marks[j] = temp;
            }
        }
    }

    // Add the best applicable marks
    for (i = 0; i < bestSubjects; i++)
    {
        if (marks[i] >= 0)
        {
            total = total + marks[i];
            subjectsTaken++;
        }
    }

    printf("Best-subject total: %d\n", total);

    if (subjectsTaken == 0)
    {
        printf("Percentage: Not available\n");
    }
    else
    {
        percentage = (float)total / subjectsTaken;
        printf("Percentage: %.2f%%\n", percentage);
    }
}

// Display one student's details
void displayStudent(int index)
{
    int i;

    printf("\n-----------------------------\n");
    printf("Roll number: %d\n", students[index].roll);
    printf("Name: %s\n", students[index].name);
    printf("Class: %s\n", students[index].studentClass);

    printf("\nSubject marks:\n");

    for (i = 0; i < SUBJECTS; i++)
    {
        printf("%s: ", subjectNames[i]);

        if (students[index].marks[i] == -1)
        {
            printf("N/A\n");
        }
        else
        {
            printf("%d\n", students[index].marks[i]);
        }
    }

    showResults(index);

    printf("-----------------------------\n");
}

// Add a new student
void addStudent()
{
    int roll, i, mark;

    if (studentCount >= MAX)
    {
        printf("Student list is full!\n");
        return;
    }

    printf("Enter roll number: ");
    scanf("%d", &roll);

    if (findStudent(roll) != -1)
    {
        printf("This roll number already exists!\n");
        return;
    }

    students[studentCount].roll = roll;

    printf("Enter student name: ");
    scanf(" %49[^\n]", students[studentCount].name);

    printf("Enter class/section: ");
    scanf(" %19[^\n]", students[studentCount].studentClass);

    for (i = 0; i < SUBJECTS; i++)
    {
        printf("%s marks (0-100, -1 for N/A): ",
               subjectNames[i]);

        scanf("%d", &mark);

        while (mark < -1 || mark > 100)
        {
            printf("Enter 0-100 or -1 for N/A: ");
            scanf("%d", &mark);
        }

        students[studentCount].marks[i] = mark;
    }

    studentCount++;

    saveData();

    printf("Student added successfully!\n");
}

// View students in groups of 10
void viewStudents()
{
    int start = 0;
    int end, i;
    char next;

    if (studentCount == 0)
    {
        printf("No students found!\n");
        return;
    }

    while (start < studentCount)
    {
        end = start + PAGE_SIZE;

        if (end > studentCount)
        {
            end = studentCount;
        }

        printf("\nStudents %d to %d\n", start + 1, end);

        for (i = start; i < end; i++)
        {
            printf("Roll: %d | Name: %s | Class: %s\n",
                   students[i].roll,
                   students[i].name,
                   students[i].studentClass);
        }

        if (end == studentCount)
        {
            break;
        }

        printf("Press N for next 10 students: ");
        scanf(" %c", &next);

        if (next != 'N' && next != 'n')
        {
            break;
        }

        start = end;
    }
}

// Search for a student
void searchStudent()
{
    int roll, index;

    printf("Enter roll number to search: ");
    scanf("%d", &roll);

    index = findStudent(roll);

    if (index == -1)
    {
        printf("Student not found!\n");
    }
    else
    {
        displayStudent(index);
    }
}

// Edit student details
void editStudent()
{
    int roll, index, choice;
    int subject, mark;

    printf("Enter roll number to edit: ");
    scanf("%d", &roll);

    index = findStudent(roll);

    if (index == -1)
    {
        printf("Student not found!\n");
        return;
    }

    printf("1. Change name\n");
    printf("2. Change class\n");
    printf("3. Change subject marks\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter new name: ");
        scanf(" %49[^\n]", students[index].name);
    }
    else if (choice == 2)
    {
        printf("Enter new class: ");
        scanf(" %19[^\n]", students[index].studentClass);
    }
    else if (choice == 3)
    {
        int i;

        for (i = 0; i < SUBJECTS; i++)
        {
            printf("%d. %s\n", i + 1, subjectNames[i]);
        }

        printf("Enter subject number (1-10): ");
        scanf("%d", &subject);

        if (subject < 1 || subject > SUBJECTS)
        {
            printf("Invalid subject number!\n");
            return;
        }

        printf("Enter new marks (0-100, -1 for N/A): ");
        scanf("%d", &mark);

        while (mark < -1 || mark > 100)
        {
            printf("Invalid marks. Try again: ");
            scanf("%d", &mark);
        }

        students[index].marks[subject - 1] = mark;
    }
    else
    {
        printf("Invalid choice!\n");
        return;
    }

    saveData();

    printf("Student updated successfully!\n");
}

// Delete a student
void deleteStudent()
{
    int roll, index, i;

    printf("Enter roll number to delete: ");
    scanf("%d", &roll);

    index = findStudent(roll);

    if (index == -1)
    {
        printf("Student not found!\n");
        return;
    }

    for (i = index; i < studentCount - 1; i++)
    {
        students[i] = students[i + 1];
    }

    studentCount--;

    saveData();

    printf("Student deleted successfully!\n");
}

// Change the number of best subjects
void changeScheme()
{
    int number;

    printf("Current best subjects: %d\n", bestSubjects);
    printf("Enter number of best subjects (1-10): ");
    scanf("%d", &number);

    if (number >= 1 && number <= SUBJECTS)
    {
        bestSubjects = number;
        saveData();

        printf("Scheme updated successfully!\n");
    }
    else
    {
        printf("Enter a number between 1 and 10!\n");
    }
}

// Simple login system
int login()
{
    char username[30];
    char password[30];

    printf("\nUsername (admin/teacher): ");
    scanf("%29s", username);

    printf("Password: ");
    scanf("%29s", password);

    if (strcmp(username, "admin") == 0 &&
        strcmp(password, "admin123") == 0)
    {
        printf("Principal login successful!\n");
        return 1;
    }

    if (strcmp(username, "teacher") == 0 &&
        strcmp(password, "teach123") == 0)
    {
        printf("Teacher login successful!\n");
        return 2;
    }

    printf("Incorrect username or password!\n");
    return 0;
}

// Main function
int main()
{
    int userType, choice;

    loadData();

    printf("===== SCHOOL MANAGEMENT SYSTEM =====\n");

    userType = login();

    if (userType == 0)
    {
        printf("Login failed. Program closed.\n");
        return 0;
    }

    while (1)
    {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Add student\n");
        printf("2. View all students\n");
        printf("3. Search student\n");
        printf("4. Edit student\n");
        printf("5. Delete student\n");

        if (userType == 1)
        {
            printf("6. Change percentage scheme\n");
        }

        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            addStudent();
        }
        else if (choice == 2)
        {
            viewStudents();
        }
        else if (choice == 3)
        {
            searchStudent();
        }
        else if (choice == 4)
        {
            editStudent();
        }
        else if (choice == 5)
        {
            deleteStudent();
        }
        else if (choice == 6 && userType == 1)
        {
            changeScheme();
        }
        else if (choice == 0)
        {
            saveData();
            printf("Data saved. Goodbye!\n");
            break;
        }
        else
        {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
