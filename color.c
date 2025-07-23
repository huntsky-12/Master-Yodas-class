#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BOLD "\033[1m"
#define UL "\033[4m"

#define MAX_USERNAME_LEN 20
#define MAX_PASSWORD_LEN 64
#define MAX_NAME_LEN 15
#define MAX_DEPARTMENT_LEN 30
#define MAX_SECURITY_QUESTION_LEN 50
#define MAX_FILE_LINE_LEN 256
#define MAX_COURSES_PER_SEMESTER 6
#define MAX_COURSE_CODE_LEN 600
#define MAX_SEMESTER_LEN 6
#define MAX_COURSE_NAME_LEN 600
#define MAX_FACULTY_NAME_LEN 30
#define MAX_TYPE_LEN 15
#define MAX_COURSES 100
#define MIN_COURSES 4
#define MAX_COURSE_TYPE_LEN 2
#define MAX_PREFERENCES
#define MAX_LINE_LENGTH 1024
#define MAX_COLUMNS 100
#define MAX_COURSE_LENGTH 50
#define MAX_STUDENTS 100
#define MAX_SECTIONS 3
#define MAX_LENGTH 100


#define MAX_COURSE_NAME 100
#define MAX_FIELD_LEN 100
#define MAX_LINE_LEN 500

struct User {
  char username[MAX_USERNAME_LEN];
  char password[MAX_PASSWORD_LEN];
  char first_name[MAX_NAME_LEN];
  char last_name[MAX_NAME_LEN];
  char department[MAX_DEPARTMENT_LEN];
  int semester;
  float cgpa;
  char security_question1[MAX_SECURITY_QUESTION_LEN];
  char security_question2[MAX_SECURITY_QUESTION_LEN];
  char courses[MAX_COURSES_PER_SEMESTER][MAX_COURSE_CODE_LEN];
  int num_courses;
  char selected_course_codes[3 * 2][MAX_COURSE_CODE_LEN];
  char selected_course_names[MAX_COURSES][MAX_COURSE_NAME_LEN];
  char related_interests[MAX_LENGTH];
};

void login_student();
void login_admin();
void new_reg_student();
void new_reg_admin();
void forget_password();
bool is_weak_password(const char *password);
bool is_valid_input(const char *input, int max_length);
bool is_username_existing(const char *username);
void hash_password(const char *password, char *hashed_password);
void choice_filling(const char *username);
void print_or_fill_choices(const char *username);
void add_or_drop_courses(const char *username);
int compare(const void *a, const void *b);
int femain();
void admin_work();
void add_or_drop_course_admin();
void allotment();
int count_profiles(const char *);
void allotment();
void display_result(const char *username);
void del_course_admin();
void add_course_admin();
bool get_user_details(const char *username, struct User *user);
void call_recommend(const char *username);
void search_faculty();
int is_student_in_waitlist(const char *username, const char *waitlist_file);
void waitlist_final();
int is_student_in_allotedlist(const char *username, const char *alloted_file);
void rank_students();
void reallocate();
int correct_file();

int main() {
  int user;
  int user_type;
  printf(BOLD GREEN "LOGIN: \n" RESET);
  printf(BOLD YELLOW "Enter 1 for Student. \n" RESET);
  printf(BOLD YELLOW "Enter 2 for Admin. \n" RESET);
  scanf("%d", &user);

  if (user == 1) {
    printf(BOLD YELLOW "Enter 1 for New User. \n" RESET);
    printf(BOLD YELLOW "Enter 2 for Existing User. \n" RESET);
    scanf("%d", &user_type);

    if (user_type == 1) {
      new_reg_student();
    } else {
      int choice;
      printf(BOLD YELLOW "Enter 1 for Login. \n" RESET);
      printf(BOLD YELLOW "Enter 2 for Forget Password. \n" RESET);
      scanf("%d", &choice);
      if (choice == 1) {
        login_student();
      } else if (choice == 2) {
        forget_password();
      } else {
        printf(RED "Invalid choice.\n" RESET);
      }
    }

  } else if (user == 2) {
    printf(BOLD YELLOW "Enter 1 for New User. \n" RESET);
    printf(BOLD YELLOW "Enter 2 for Existing User. \n" RESET);
    scanf("%d", &user_type);

    if (user_type == 1) {
      new_reg_admin();
    } else {
      int choice;
      printf(BOLD YELLOW "Enter 1 for Login. \n" RESET);
      printf(BOLD YELLOW "Enter 2 for Forget Password. \n" RESET);
      scanf("%d", &choice);
      if (choice == 1) {
        login_admin();
      } else if (choice == 2) {
        forget_password();
      } else {
        printf(RED "Invalid choice.\n" RESET);
      }
    }
  } else {
    printf(RED "Invalid choice.\n" RESET);
  }

  return 0;
}

void login_student() {
  FILE *file = fopen("data.csv", "r");
  if (file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return;
  }

  struct User user;
  char username[MAX_USERNAME_LEN];
  char password[MAX_PASSWORD_LEN];
  printf(BOLD CYAN "Enter the username: " RESET);
  scanf("%19s", username);
  printf(BOLD CYAN "Enter the password: " RESET);
  scanf("%63s", password);

  char hashed_password[MAX_PASSWORD_LEN];
  hash_password(password, hashed_password);

  while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f,%[^,],%[^\n]\n",
                user.username, user.password, user.first_name, user.last_name,
                user.department, &user.semester, &user.cgpa,
                user.security_question1, user.security_question2) == 9) {
    if (strcmp(username, user.username) == 0 &&
        strcmp(hashed_password, user.password) == 0) {
      printf(BOLD GREEN "Successfully logged in!!!\n" RESET);
      fclose(file);
      print_or_fill_choices(username);
      return;
    }
  }

  printf(RED "Invalid credentials\n" RESET);
  fclose(file);
}

struct Student_Ranks {
  char username[50];
  char firstName[50];
  char lastName[50];
  float cgpa;
  char branch[10];
  int semester;
  char courses[MAX_COLUMNS][50];
  int numCourses;
};

// Function to compare two students based on CGPA (for qsort)
int compare(const void *a, const void *b) {
  const struct Student_Ranks *studentA = (const struct Student_Ranks *)a;
  const struct Student_Ranks *studentB = (const struct Student_Ranks *)b;

  if (studentA->cgpa > studentB->cgpa) {
    return -1; // Higher CGPA comes first
  } else if (studentA->cgpa < studentB->cgpa) {
    return 1;
  } else {
    return 0;
  }
}

// Correct function structure for femain()
int femain() {
  FILE *input_file = fopen("choice_list.csv", "r");
  FILE *output_file = fopen("sorted_list.csv", "w");

  if (input_file == NULL || output_file == NULL) {
    printf(RED "Error opening files.\n" RESET);
    return 1;
  }

  char line[MAX_LINE_LENGTH];
  struct Student_Ranks students[MAX_COLUMNS];
  int num_students = 0;

  // Read lines from input file
  while (fgets(line, sizeof(line), input_file) != NULL) {
    // Parse the line and store student information
    char courses[MAX_LINE_LENGTH];
    sscanf(line, "%[^,],%[^,],%[^,],%f,%[^,],%d,%[^\n]",
           students[num_students].username, students[num_students].firstName,
           students[num_students].lastName, &students[num_students].cgpa,
           students[num_students].branch, &students[num_students].semester,
           courses);

    // Split courses into separate strings
    char *course = strtok(courses, ",");
    students[num_students].numCourses = 0;
    while (course != NULL && students[num_students].numCourses < MAX_COLUMNS) {
      strcpy(students[num_students].courses[students[num_students].numCourses],
             course);
      students[num_students].numCourses++;
      course = strtok(NULL, ",");
    }

    // Increment number of students read
    num_students++;
  }

  // Sort students based on CGPA
  qsort(students, num_students, sizeof(struct Student_Ranks), compare);

  // Write sorted students to output file
  for (int i = 0; i < num_students; i++) {
    fprintf(output_file, "%s,%s,%s,%.2f,%s,%d,%s", students[i].username,
            students[i].firstName, students[i].lastName, students[i].cgpa,
            students[i].branch, students[i].semester, students[i].courses[0]);

    // Add commas for remaining courses
    for (int j = 1; j < students[i].numCourses; j++) {
      fprintf(output_file, ",%s,", students[i].courses[j]);
    }

    fprintf(output_file, "\n");
  }

  // Close files
  fclose(input_file);
  fclose(output_file);

  printf(
      BOLD GREEN
      "Student ranks successfully sorted and saved to sorted_list.csv\n" RESET);
  return 0;
}

void login_admin() {
  FILE *file = fopen("data_admin.csv", "r");
  if (file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return;
  }

  struct User user;
  char username[MAX_USERNAME_LEN];
  char password[MAX_PASSWORD_LEN];
  printf(BOLD CYAN "Enter the username: " RESET);
  scanf("%19s", username);
  printf(BOLD CYAN "Enter the password: " RESET);
  scanf("%63s", password);

  char hashed_password[MAX_PASSWORD_LEN];
  hash_password(password, hashed_password);

  while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", user.username,
                user.password, user.first_name, user.last_name,
                user.security_question1, user.security_question2) == 6) {
    if (strcmp(username, user.username) == 0 &&
        strcmp(hashed_password, user.password) == 0) {
      printf(BOLD GREEN "Successfully logged in!!!\n" RESET);
      fclose(file);
      admin_work();

      return; // Exit
    }
  }

  printf(RED "Invalid credentials\n" RESET);
  fclose(file);
}
int final_allotment_done = 0;

void admin_work() {
  int choice;
  int choice2;

  while (1) { // Loop to keep the admin work menu running
    printf("\n");
    printf(GREEN BOLD UL "  COURSE ADMINISTRATION   \n" RESET);
    printf("\n");
    printf(YELLOW "1. Add or Delete a course.\n" RESET);
    printf(YELLOW "2. Rank the students\n" RESET);
    printf(YELLOW "3. Proceed allotment\n" RESET);
    printf(YELLOW "4. Rank waitlisted students\n" RESET);
    printf(YELLOW "5. Reallocate\n" RESET);
    printf(YELLOW "6. Exit\n" RESET); // Option to exit the loop

    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
      printf(RED "Invalid input. Please enter a number.\n" RESET);
      while (getchar() != '\n')
        ; // Clear the input buffer
      continue;
    }

    switch (choice) {
    case 1:
      printf("\n");
      printf(GREEN BOLD UL "ADDITION & DELETION OF COURSE:\n" RESET);
      printf(YELLOW BOLD "1. Add a course.\n" RESET);
      printf(YELLOW BOLD "2. Delete a course.\n" RESET);

      printf("Enter your choice: ");
      if (scanf("%d", &choice2) != 1) {
        printf(RED "Invalid input. Please enter a number.\n" RESET);
        while (getchar() != '\n')
          ; // Clear the input buffer
        continue;
      }

      if (choice2 == 1) {
        add_course_admin();
      } else if (choice2 == 2) {
        del_course_admin();
      } else {
        printf(RED "Invalid input\n" RESET);
      }
      break;

    case 2:
      printf("Ranking the students...\n"); // Debugging statement
      femain();
      break;

    case 3:
      printf("Proceeding with allotment...\n"); // Debugging statement
      allotment();
      break;

    case 4:
      printf("Ranking waitlisted students...\n"); // Debugging statement
      rank_students();
      break;

    case 5:
      if (!final_allotment_done) {
        printf(
            "Proceeding with final allotment and reallocation...\n"); // Debugging
                                                                      // statement
        reallocate();

        final_allotment_done = 1;
        printf(RED "Final allotment done. Students cannot change course "
                   "choices now.\n" RESET);
      } else {
        printf(RED "Final allotment has already been done. You cannot "
                   "reallocate again.\n" RESET);
      }
      break;

    case 6:
      printf(GREEN BOLD "LOGGED OUT\n" RESET);
      return; // Exit the loop

    default:
      printf(RED "Invalid choice\n" RESET);
      break;
    }
  }
}

void new_reg_student() {
  FILE *file = fopen("data.csv", "a+");
  if (file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return;
  }

  struct User user;
  printf(BOLD UL GREEN "   REGISTRATION:    \n" RESET);
  printf("\n");
  printf("\n");
  printf(BOLD UL GREEN "\nWelcome to the online university course registration "
                       "system!!\n" RESET);
  printf(CYAN "Enter the username: " RESET);
  scanf("%19s", user.username);
  while (!is_valid_input(user.username, MAX_USERNAME_LEN - 1) ||
         is_username_existing(user.username)) {
    if (!is_valid_input(user.username, MAX_USERNAME_LEN - 1)) {
      printf(RED "Invalid username. " RESET);
    } else {
      printf(RED "Username already exists. " RESET);
    }
    printf(CYAN "Please enter a valid username: " RESET);
    scanf("%19s", user.username);
  }

  printf(CYAN "Enter the password: " RESET);
  scanf("%63s", user.password);
  while (is_weak_password(user.password)) {
    printf(RED "Weak password. Please enter a stronger password: " RESET);
    scanf("%63s", user.password);
  }

  printf(CYAN "First name: " RESET);
  scanf("%14s", user.first_name);
  while (!is_valid_input(user.first_name, MAX_NAME_LEN - 1)) {
    printf(RED "Invalid first name. Please enter a valid first name: " RESET);
    scanf("%14s", user.first_name);
  }

  printf(CYAN "Last name: " RESET);
  scanf("%14s", user.last_name);
  while (!is_valid_input(user.last_name, MAX_NAME_LEN - 1)) {
    printf(RED "Invalid last name. Please enter a valid last name: " RESET);
    scanf("%14s", user.last_name);
  }

  printf(CYAN "Enter your department :\n" RESET);
  printf(CYAN "Computer Science engineering: CSE\n" RESET);
  printf(CYAN "Mechanical Engineering: ME\n" RESET);
  printf(CYAN "Electronics and Communication Engineering: ECE\n" RESET);
  printf(CYAN "Electrical Engineering : EE\n" RESET);
  printf(CYAN "Civil Engineering : CE\n" RESET);
  
  scanf("%29s", user.department);
  while (!is_valid_input(user.department, MAX_DEPARTMENT_LEN - 1)) {
    printf(RED "Invalid department. Please enter a valid department: " RESET);
    scanf("%29s", user.department);
  }

  printf(CYAN "Enter the upcoming semester: " RESET);
  scanf("%d", &user.semester);
  while (user.semester < 2 || user.semester > 8) {
    printf(RED "Invalid semester. Please enter a valid semester: " RESET);
    scanf("%d", &user.semester);
  }

  printf(CYAN "Enter your last semester CGPA: " RESET);
  scanf("%f", &user.cgpa);
  while (user.cgpa < 0.0 || user.cgpa > 10.0) {
    printf(RED "Invalid CGPA. Please enter a valid CGPA: " RESET);
    scanf("%f", &user.cgpa);
  }

  printf(GREEN "Security Question for forget password\n" RESET);
  printf(CYAN "What's your pet name? " RESET);
  scanf("%49s", user.security_question1);
  printf(CYAN "What's your favourite singer name? " RESET);
  scanf("%49s", user.security_question2);

  char hashed_password[MAX_PASSWORD_LEN];
  hash_password(user.password, hashed_password);

  fprintf(file, "%s,%s,%s,%s,%s,%d,%f,%s,%s\n", user.username, hashed_password,
          user.first_name, user.last_name, user.department, user.semester,
          user.cgpa, user.security_question1, user.security_question2);

  fclose(file);
  printf(GREEN "Registration successful\n" RESET);
  login_student();
}

void new_reg_admin() {
  FILE *file = fopen("data_admin.csv", "a+");
  if (file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return;
  }

  struct User user;
  printf(BOLD UL GREEN "   REGISTRATION:    \n" RESET);
  printf("\n");
  printf("\n");
  printf(
      BOLD GREEN UL
      "Welcome to the online university course registration system!!\n" RESET);
  printf(BOLD CYAN "Enter the username: " RESET);
  scanf("%19s", user.username);
  while (!is_valid_input(user.username, MAX_USERNAME_LEN - 1)) {
    printf(RED "Invalid username. Please enter a valid username: " RESET);
    scanf("%19s", user.username);
  }

  printf(BOLD CYAN "Enter the password: " RESET);
  scanf("%63s", user.password);
  while (is_weak_password(user.password)) {
    printf(RED "Weak password. Please enter a stronger password: " RESET);
    scanf("%63s", user.password);
  }

  printf(CYAN "First name: " RESET);
  scanf("%14s", user.first_name);
  while (!is_valid_input(user.first_name, MAX_NAME_LEN - 1)) {
    printf(RED "Invalid first name. Please enter a valid first name: " RESET);
    scanf("%14s", user.first_name);
  }

  printf(CYAN "Last name: " RESET);
  scanf("%14s", user.last_name);
  while (!is_valid_input(user.last_name, MAX_NAME_LEN - 1)) {
    printf(RED "Invalid last name. Please enter a valid last name: " RESET);
    scanf("%14s", user.last_name);
  }

  printf(BOLD GREEN "Security Question for forget password\n" RESET);
  printf(CYAN "What's your pet name? " RESET);
  scanf("%49s", user.security_question1);
  printf(CYAN "What's your favourite singer name? " RESET);
  scanf("%49s", user.security_question2);

  char hashed_password[MAX_PASSWORD_LEN];
  hash_password(user.password, hashed_password);

  fprintf(file, "%s,%s,%s,%s,%s,%s\n", user.username, hashed_password,
          user.first_name, user.last_name, user.security_question1,
          user.security_question2);

  fclose(file);
  printf(BOLD GREEN "Registration successful\n" RESET);
  login_admin();
}
void forget_password() {
  char username[MAX_USERNAME_LEN];
  printf(CYAN "Enter the username: " RESET);
  scanf("%19s", username);

  // Open the file in read mode
  FILE *file = fopen("data.csv", "r");
  if (file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return;
  }

  struct User user;
  bool user_found = false;
  char line[MAX_FILE_LINE_LEN];

  // Find the user in the file
  while (fgets(line, sizeof(line), file)) {
    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f,%[^,],%[^\n]\n",
           user.username, user.password, user.first_name, user.last_name,
           user.department, &user.semester, &user.cgpa, user.security_question1,
           user.security_question2);

    if (strcmp(username, user.username) == 0) {
      user_found = true;
      break;
    }
  }

  fclose(file);

  if (!user_found) {
    printf(RED "Username not found.\n" RESET);
    return;
  }

  // Ask security questions
  char answer1[MAX_SECURITY_QUESTION_LEN];
  char answer2[MAX_SECURITY_QUESTION_LEN];
  printf(BOLD YELLOW
         "Answer the security questions to reset your password.\n" RESET);
  printf(CYAN "What's your pet name? " RESET);
  scanf("%49s", answer1);
  printf(CYAN "What's your favourite singer name? " RESET);
  scanf("%49s", answer2);

  // Validate security answers
  if (strcmp(answer1, user.security_question1) != 0 ||
      strcmp(answer2, user.security_question2) != 0) {
    printf(RED "Security answers do not match.\n" RESET);
    return;
  }

  // Allow user to set a new password
  char new_password[MAX_PASSWORD_LEN];
  printf(CYAN "Enter a new password: " RESET);
  scanf("%63s", new_password);

  while (is_weak_password(new_password)) {
    printf(RED "Weak password. Please enter a stronger password: " RESET);
    scanf("%63s", new_password);
  }

  char hashed_password[MAX_PASSWORD_LEN];
  hash_password(new_password, hashed_password);

  file = fopen("data.csv", "r");
  if (file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return;
  }
  FILE *temp_file = fopen("temp.csv", "w");
  if (temp_file == NULL) {
    printf(RED "Error creating temporary file.\n" RESET);
    fclose(file);
    return;
  }

  while (fgets(line, sizeof(line), file)) {
    sscanf(line, "%[^,],", user.username);

    if (strcmp(username, user.username) == 0) {
      // Write the updated user data
      fprintf(temp_file, "%s,%s,%s,%s,%s,%d,%f,%s,%s\n", user.username,
              hashed_password, user.first_name, user.last_name, user.department,
              user.semester, user.cgpa, user.security_question1,
              user.security_question2);
    } else {
      // Write the original line
      fputs(line, temp_file);
    }
  }

  fclose(file);
  fclose(temp_file);

  // Replace the original file with the updated file
  remove("data.csv");
  rename("temp.csv", "data.csv");

  printf(GREEN "Password reset successfully.\n" RESET);
}

bool is_weak_password(const char *password) {
  if (strlen(password) < 8) {
    return true;
  }
  bool has_upper = false, has_lower = false, has_digit = false,
       has_special = false;
  while (*password) {
    if (isupper(*password))
      has_upper = true;
    else if (islower(*password))
      has_lower = true;
    else if (isdigit(*password))
      has_digit = true;
    else
      has_special = true;
    password++;
  }
  return !(has_upper && has_lower && has_digit && has_special);
}

bool is_username_existing(const char *username) {
  FILE *file = fopen("data.csv", "r");
  if (file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return false;
  }

  struct User user;
  char line[MAX_FILE_LINE_LEN];

  while (fgets(line, sizeof(line), file)) {
    sscanf(line, "%[^,],%*s", user.username); // Read only the username
    if (strcmp(username, user.username) == 0) {
      fclose(file);
      return true; // Username exists
    }
  }

  fclose(file);
  return false; // Username does not exist
}

bool is_valid_input(const char *input, int max_length) {
  if (strlen(input) > max_length)
    return false;
  while (*input) {
    if (!isalnum(*input) && *input != '_')
      return false;
    input++;
  }
  return true;
}

void hash_password(const char *password, char *hashed_password) {
  strncpy(hashed_password, password, MAX_PASSWORD_LEN - 1);
  hashed_password[MAX_PASSWORD_LEN - 1] = '\0';
  // Replace with real hashing later
}

#define MAX_INTERESTS 100


struct Course1 {
    char department[10];
    int semester;
    char course_code[MAX_COURSE_CODE_LEN];
    char sec_course_code[MAX_COURSE_CODE_LEN];
    char type[20];
    char course_name[50];
    float req_cgpa;
    float credit;
    char faculty_name[50];
    char related_interests[MAX_INTERESTS];
};




void call_recommend(const char *username) {
    FILE *file = fopen("list.csv", "r");
    if (!file) {
        printf(RED "Error opening list.csv file.\n" RESET);
        return;
    }

    struct User user;
    if (!get_user_details(username, &user)) {
        printf(RED "Error retrieving user details.\n" RESET);
        fclose(file);
        return;
    }

    printf(YELLOW "\n Logged in as: %s | Dept: %s | Sem: %d | CGPA: %.2f\n" RESET,
           user.username, user.department, user.semester, user.cgpa);

    struct Course1 courses[MAX_COURSES];
    int course_count = 0;
    char line[MAX_LINE_LENGTH];

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        struct Course1 course;

        int parsed = sscanf(line,
            "%[^,],%d,%[^,],%[^,],%[^,],%[^,],%f,%f,%[^,],%[^\n]",
            course.department,
            &course.semester,
            course.sec_course_code,
            course.course_code,
            course.type,
            course.course_name,
            &course.req_cgpa,
            &course.credit,
            course.faculty_name,
            course.related_interests
        );

        if (parsed != 10) continue;

        if (strcasecmp(course.department, user.department) == 0 &&
            course.semester == user.semester &&
            user.cgpa >= course.req_cgpa) {
            
            courses[course_count++] = course;
        }
    }

    fclose(file);

    if (course_count == 0) {
        printf(RED "You are not eligible for any courses.\n" RESET);
        return;
    }

    bool recommendations[MAX_COURSES] = {false};
    char asked_interests[MAX_COURSES][MAX_INTERESTS] = {{0}};
    int asked_count = 0;

    for (int i = 0; i < course_count; i++) {
        if (strlen(courses[i].related_interests) == 0) continue;

        bool already_asked = false;
        for (int j = 0; j < asked_count; j++) {
            if (strcmp(asked_interests[j], courses[i].related_interests) == 0) {
                already_asked = true;
                break;
            }
        }

        if (already_asked) continue;

        char response[10];
        printf(YELLOW "Do you have an interest in %s? (yes/no): " RESET,
               courses[i].related_interests);
        scanf("%9s", response);

        for (int k = 0; response[k]; k++) {
            response[k] = tolower(response[k]);
        }

        if (strcmp(response, "yes") == 0) {
            for (int j = 0; j < course_count; j++) {
                if (strcmp(courses[j].related_interests, courses[i].related_interests) == 0) {
                    recommendations[j] = true;
                }
            }
        }

        strncpy(asked_interests[asked_count++],
                courses[i].related_interests, MAX_INTERESTS - 1);
    }

    printf(GREEN "\nRecommended Courses:\n" RESET);
    int displayed = 0;

    for (int i = 0; i < course_count; i++) {
        if (recommendations[i]) {
            printf(WHITE "-- %s (%s), Section: %s, Faculty: %s, Credits: %.1f, Interest: %s\n" RESET,
                   courses[i].course_name,
                   courses[i].course_code,
                   courses[i].sec_course_code,
                   courses[i].faculty_name,
                   courses[i].credit,
                   courses[i].related_interests);
            displayed++;
        }
    }

    if (displayed == 0) {
        printf(RED "No recommended courses found based on your interests.\n" RESET);
    }
}



bool get_user_details(const char *username, struct User *user) {
  FILE *file = fopen("data.csv", "r");
  if (file == NULL) {
    printf(RED "Error opening data.csv file.\n" RESET);
    return false;
  }

  char line[MAX_LINE_LENGTH];
  bool found = false;

  while (fgets(line, sizeof(line), file)) {
    sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%d,%f,%[^,],%[^\n]",
           user->username, user->password, user->first_name, user->last_name,
           user->department, &user->semester, &user->cgpa,
           user->security_question1, user->security_question2);

    if (strcmp(username, user->username) == 0) {
      found = true;
      break;
    }
  }

  fclose(file);
  return found;
}
struct Course {
  char code[MAX_COURSE_CODE_LEN];
  char name[MAX_COURSE_NAME_LEN];
  char department[MAX_DEPARTMENT_LEN];
  char semester[10];
  char details[3024];
  char course_Type[15];
};

void print_or_fill_choices(const char *username) {
  if (final_allotment_done) {
    printf("Course choices cannot be modified after final allotment.\n");
    return;
  }
  FILE *choice_file = fopen("choice_list.csv", "r");
  if (choice_file == NULL) {
    printf(RED "Error opening choice list file.\n" RESET);
    return;
  }
  struct User user;
  char buffer[1024];
  int found = 0;

  while (fgets(buffer, sizeof(buffer), choice_file)) {
    // Remove the trailing newline character
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }

    // Parse the line
    char user_name[MAX_USERNAME_LEN], first_name[50], last_name[50],
        department[10];
    float cgpa;
    int semester;
    char choices[18][MAX_COURSE_CODE_LEN] = {
        ""}; // Adjusted for up to 6 courses with 3 preferences each
    int num_read =
        sscanf(buffer,
               "%49[^,],%49[^,],%49[^,],%f,%9[^,],%d,"
               "%19[^,],%19[^,],%19[^,],%19[^,],%19[^,],%19[^,],"
               "%19[^,],%19[^,],%19[^,],%19[^,],%19[^,],%19[^,],"
               "%19[^,],%19[^,],%19[^,],%19[^,],%19[^,],%19[^,],",
               user_name, first_name, last_name, &cgpa, department, &semester,
               choices[0], choices[1], choices[2], choices[3], choices[4],
               choices[5], choices[6], choices[7], choices[8], choices[9],
               choices[10], choices[11], choices[12], choices[13], choices[14],
               choices[15], choices[16], choices[17]);

    if (strcmp(user_name, username) == 0) {
      printf(BOLD GREEN "Choices made by %s %s:\n" RESET, first_name,
             last_name);

      for (int i = 0; i < 18; i += 3) {
        if (choices[i][0] != '\0') {
          int course_num = (i / 3) + 1;
          printf(YELLOW "Course %d:\n" RESET, course_num);
          printf(YELLOW "  Preference 1: %s\n" RESET, choices[i]);
          printf(YELLOW "  Preference 2: %s\n" RESET, choices[i + 1]);
          printf(YELLOW "  Preference 3: %s\n" RESET, choices[i + 2]);
        }
      }

      found = 1;
      break;
    }
  }
  fclose(choice_file);

  if (!found) {
    printf(RED "No previous choices found for %s.\n" RESET, username);
    printf(YELLOW "Proceeding to course selection...\n" RESET);
    char filename[] = "list.csv";
    call_recommend(username);
    int choicemake;
    while (choicemake != 3) {
      printf("1.To proceed to choicefilling\n");
      printf("2.To Enquire about Faculty.\n");
      printf("3.Exit\n");
      scanf("%d", &choicemake);
      if (choicemake == 1) {
        choice_filling(username);
      } else if (choicemake == 2) {
        search_faculty();
      } else if (choicemake == 3) {
        printf("Thank you for using our system\n");
        break;
      } else {
        printf("Invalid choice\n");
      }
    }

  } else {
    display_result(username);

    if (is_student_in_waitlist(username, "Waitlist.csv")) {
      waitlist_final(username);
    }
    if (is_student_in_allotedlist(username, "Allotment.csv")) {
      add_or_drop_courses(username);
    }
  }
}

void choice_filling(const char *username) {
    FILE *file = fopen("data.csv", "r");
    if (file == NULL) {
        printf("Error opening data file.\n");
        return;
    }

    struct User user;
    int found = 0;
    while (fscanf(file, "%19[^,],%19[^,],%99[^,],%99[^,],%49[^,],%d,%f,%99[^,],%99[^\n]\n",
                  user.username, user.password, user.first_name, user.last_name,
                  user.department, &user.semester, &user.cgpa,
                  user.security_question1, user.security_question2) == 9) {
        if (strcmp(username, user.username) == 0) {
            found = 1;
            break;
        }
    }
    fclose(file);
    if (!found) {
        printf("User not found.\n");
        return;
    }

    FILE *course_file = fopen("list.csv", "r");
    if (course_file == NULL) {
        printf(RED "Error opening course list file.\n" RESET);
        return;
    }

    printf(MAGENTA "Welcome, %s! Choose your courses for the semester.\n" RESET, user.first_name);
    
    char course_code[MAX_COURSE_CODE_LEN];
    char course_name[100];
    char faculty_name[100];
    char department[MAX_DEPARTMENT_LEN];
    char semester[MAX_SEMESTER_LEN];
    char buffer[9000];
    char unique_course_code[1024];
    char course_type[10];
    int credit;
    char course_details[200];
    float course_req_cgpa;

    struct Course courses[MAX_COURSES];
    int num_courses = 0;

    // Skip header
    fgets(buffer, sizeof(buffer), course_file);

    // Load Core courses
    printf(BOLD UL GREEN "        Available Core courses:\n" RESET);
    printf(BOLD MAGENTA "SECTION CODE    COURSE NAME\n" RESET);
    while (fgets(buffer, sizeof(buffer), course_file)) {
        sscanf(buffer, "%[^,],%[^,],%[^,],%*[^,],%[^,],%[^,],%f,%*f,%[^,],%[^\n]",
               department, semester, course_code, course_type,
               course_name, &course_req_cgpa, faculty_name, course_details);

        if (strcmp(user.department, department) == 0 &&
            user.semester == atoi(semester) &&
            strcmp(course_type, "Core") == 0 &&
            user.cgpa >= course_req_cgpa) {
            printf(BLUE "%-15s %s\n" RESET, course_code, course_name);
            strncpy(courses[num_courses].code, course_code, MAX_COURSE_CODE_LEN - 1);      // section code
            strncpy(courses[num_courses].name, course_name, MAX_COURSE_NAME_LEN - 1);
            strncpy(courses[num_courses].department, department, MAX_DEPARTMENT_LEN - 1);
            strncpy(courses[num_courses].semester, semester, MAX_SEMESTER_LEN - 1);
            strncpy(courses[num_courses].course_Type, course_type, MAX_TYPE_LEN - 1);
            strncpy(courses[num_courses].details, course_details, sizeof(course_details));
            num_courses++;
        }
    }
    fclose(course_file);

    user.num_courses = 0;

    while (1) {
        if (user.num_courses >= 3) {
            char done[MAX_COURSE_CODE_LEN];
            printf(YELLOW "You have selected at least 4 core courses. Type 'done' to stop: \n" RESET);
            fgets(done, MAX_COURSE_CODE_LEN, stdin);
            done[strcspn(done, "\n")] = '\0';
            if (strcmp(done, "done") == 0) break;
        }

        if (user.num_courses >= 6) {
            printf(RED "Maximum courses selected (6).\n" RESET);
            break;
        }

        printf(GREEN "Enter the 3 preferences (section codes) for Core subject %d:\n" RESET, user.num_courses + 1);
        char pref[3][MAX_COURSE_CODE_LEN];
        for (int k = 0; k < 3; k++) {
            printf(YELLOW "Enter preference %d: " RESET, k + 1);
            scanf("%s", pref[k]);

            int already_selected = 0;
            for (int i = 0; i < user.num_courses * 3; i++) {
                if (strcmp(user.selected_course_codes[i], pref[k]) == 0) {
                    already_selected = 1;
                    break;
                }
            }
            if (already_selected) {
                printf(RED "Section %s already selected. Try another.\n" RESET, pref[k]);
                k--; continue;
            }
        }

        // ✅ FIND course name of the first preference
        char selected_course_name[MAX_COURSE_NAME_LEN] = "";
        for (int i = 0; i < num_courses; ++i) {
            if (strcmp(courses[i].code, pref[0]) == 0) {
                strncpy(selected_course_name, courses[i].name, MAX_COURSE_NAME_LEN - 1);
                break;
            }
        }

        int valid = (strlen(selected_course_name) > 0);
        for (int i = 0; i < 3 && valid; ++i) {
            int matched = 0;
            for (int j = 0; j < num_courses; ++j) {
                if (strcmp(courses[j].code, pref[i]) == 0 &&
                    strcmp(courses[j].name, selected_course_name) == 0) {
                    matched = 1;
                    break;
                }
            }
            if (!matched) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            for (int i = 0; i < 3; i++) {
                strncpy(user.selected_course_codes[user.num_courses * 3 + i], pref[i], MAX_COURSE_CODE_LEN - 1);
            }
            user.num_courses++;
            printf(GREEN "Added preferences: %s, %s, %s\n" RESET, pref[0], pref[1], pref[2]);
        } else {
            printf(RED "Invalid: All preferences must be valid sections of the same course.\n" RESET);
        }
    }

    // [Your elective course selection continues unchanged from here...]
    // Be sure to apply the same logic there, checking section codes and matching course name.
    
    // Save to choice_list.csv logic below here ...
    FILE *choice_file = fopen("choice_list.csv", "a");
    if (choice_file == NULL) {
        printf(RED "Error opening choice list file.\n" RESET);
        return;
    }
    fprintf(choice_file, "%s,%s,%s,%f,%s,%d,", user.username, user.first_name,
            user.last_name, user.cgpa, user.department, user.semester);
    for (int i = 0; i < user.num_courses; i++) {
        fprintf(choice_file, "%s,%s,%s,", user.selected_course_codes[i * 3],
                user.selected_course_codes[i * 3 + 1], user.selected_course_codes[i * 3 + 2]);
    }
    fprintf(choice_file, "\n");
    fclose(choice_file);
    printf(GREEN "Choices successfully saved.\n" RESET);
}


void add_or_drop_courses(const char *username) {
  FILE *file = fopen("Allotment.csv", "r");
  if (file == NULL) {
    printf(RED "Error opening choice list file.\n" RESET);
    return;
  }

  struct User user;
  int user_found = 0;
  char buffer[9000];
  char user_buffer[9000];
  // Username,First Name,Last Name,CGPA,Department,Courses
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    sscanf(buffer, "%49[^,],%49[^,],%49[^,],%f,%49[^,],%[^\n]", user.username,
           user.first_name, user.last_name, &user.cgpa, user.department,
           user_buffer);

    if (strcmp(username, user.username) == 0) {
      user_found = 1;
      break;
    }
  }
  fclose(file);

  if (!user_found) {
    printf(RED "Username1 not found.\n" RESET);
    return;
  }

  // Parse selected courses (only course codes)
  char *token = strtok(user_buffer, ",");
  user.num_courses = 0;
  while (token != NULL && user.num_courses < MAX_COURSES) {
    // Copy the course code into the user's selected courses
    strncpy(user.selected_course_codes[user.num_courses], token,
            MAX_COURSE_CODE_LEN - 1);
    user.selected_course_codes[user.num_courses][MAX_COURSE_CODE_LEN - 1] =
        '\0'; // Ensure null-termination
    user.num_courses++;
    token = strtok(NULL, ",");
  }
  int choice;
  while (choice != 3) {
    printf(BOLD GREEN UL "Course modification portal for student.\n" RESET);
    printf(BOLD YELLOW "1. Add courses\n" RESET);
    printf(BOLD YELLOW "2. Drop course\n" RESET);
    printf(BOLD YELLOW "3.Exit\n" RESET);

    scanf("%d", &choice);

    if (choice == 1) {
      printf("%d", user.num_courses);
      if (user.num_courses >= MAX_COURSES) {
        printf(RED "Cannot add more courses. Maximum limit reached.\n" RESET);
      } else {

        file = fopen("list.csv", "r");
        if (file == NULL) {
          printf(RED "Error opening courses file.\n" RESET);
          return;
        }
        char course_code[MAX_COURSE_CODE_LEN];
        char course_name[MAX_COURSE_NAME_LEN];
        char department[MAX_DEPARTMENT_LEN];
        char semester[MAX_SEMESTER_LEN];
        char buffer[1024];
        char unique_course_code[100];
        char course_type[15];
        float course_req_cgpa;
        char course_details[1024];

        // Read the header line of list.csv to skip it
        fgets(buffer, sizeof(buffer), file);

        printf(BOLD UL GREEN "       Available courses:     \n" RESET);
        struct Course courses[600];
        int num_courses = 0;
        while (fgets(buffer, sizeof(buffer), file)) {
          sscanf(buffer,
                 "%49[^,],%9[^,],%19[^,],%19[^,],%9[^,],%99[^,],%f,%99[^\n]",
                 department, semester, unique_course_code, course_code,
                 course_type, course_name, &course_req_cgpa, course_details);
          if (strcmp(user.department, department) == 0 &&
              user.semester == atoi(semester)) {
            printf(BLUE "%s %s\n" RESET, unique_course_code, course_code,
                   course_name);

            char *comma_pos = strchr(course_name, ',');
            if (comma_pos != NULL) {
              size_t name_len = comma_pos - course_name;
              strncpy(courses[num_courses].name, course_name, name_len);
              courses[num_courses].name[name_len] =
                  '\0'; // Null-terminate the string
            } else {
              strcpy(courses[num_courses].name, course_name);
            }

            strcpy(courses[num_courses].code, course_code);
            num_courses++;
          }
        }

        fclose(file);

        printf(BOLD YELLOW "Enter the course code to add: " RESET);
        char course_co[MAX_COURSE_CODE_LEN];
        scanf("%s", course_co);

        // Check if the course code exists in list.csv
        file = fopen("list.csv", "r");
        if (file == NULL) {
          printf(RED "Error opening courses file.\n" RESET);
          return;
        }

        struct Course course;
        bool course_found = false;
        while (fscanf(file, "%[^,],%[^,],%[^,],%[^,], %99[^\n]",
                      course.department, course.semester, course.code,
                      course.name, course.details) == 5) {
          if (strcmp(course_co, course.code) == 0) {
            bool already_selected = false;
            for (int i = 0; i < user.num_courses; i++) {
              if (strcmp(user.selected_course_codes[i], course.code) == 0) {
                already_selected = true;
                break;
              }
            }

            if (already_selected) {
              printf(RED "Course already selected.\n" RESET);
              fclose(file);
              return;
            }
            printf(BLUE "Course found: %s %s\n" RESET, course.code,
                   course.name);
            strcpy(user.selected_course_codes[user.num_courses], course.code);
            strcpy(user.selected_course_names[user.num_courses], course.name);
            user.num_courses++;
            printf(BOLD GREEN "Course added successfully.\n" RESET);
            course_found = true;
            break;
          }
        }

        fclose(file);

        if (!course_found) {
          printf(RED "Invalid course code.\n" RESET);
          return;
        }
      }
    } else if (choice == 2) {
      printf("%d", user.num_courses);
      if (user.num_courses <= 4) {
        printf(RED "No courses to drop.\n" RESET);
        return;
      }

      char course_code_to_drop[MAX_COURSE_CODE_LEN];
      printf(BOLD YELLOW "Enter the course code to drop: " RESET);
      scanf("%s", course_code_to_drop);

      // Check if the course code exists in the user's selected courses
      bool course_found = false;
      for (int i = 0; i < user.num_courses; i++) {
        if (strcmp(user.selected_course_codes[i], course_code_to_drop) == 0) {
          course_found = true;
          // Remove the course by shifting elements in the arrays
          for (int j = i; j < user.num_courses - 1; j++) {
            strcpy(user.selected_course_codes[j],
                   user.selected_course_codes[j + 1]);
            strcpy(user.selected_course_names[j],
                   user.selected_course_names[j + 1]);
          }
          user.num_courses--;
          printf(BOLD GREEN "Course dropped successfully.\n" RESET);
        }
      }

      // If course not found in selected courses, check if it exists in
      // choice_list.csv
      if (!course_found) {
        FILE *choice_file = fopen("choice_list.csv", "r");
        if (choice_file == NULL) {
          printf(RED "Error opening choice list file.\n" RESET);
          return;
        }

        struct User temp_user;
        bool course_in_choice_list = false;
        while (fgets(buffer, sizeof(buffer), choice_file) != NULL) {
          sscanf(buffer, "%49[^,],%49[^,],%49[^,],%f,%49[^,],%d,%[^\n]",
                 temp_user.username, temp_user.first_name, temp_user.last_name,
                 &temp_user.cgpa, temp_user.department, &temp_user.semester,
                 user_buffer);

          // Check if the course code is in the current line
          char *course_code_pos = strstr(user_buffer, course_code_to_drop);
          if (course_code_pos != NULL) {
            // Course code found in choice_list.csv
            course_in_choice_list = true;
            break;
          }
        }

        fclose(choice_file);

        if (course_in_choice_list) {
          printf(RED "The course is not selected.\n" RESET);
        } else {
          printf(RED "Invalid course code.\n" RESET);
        }
      }
    } else if (choice == 3) {
      printf("LOGGING OUT\n");
      break;
    } else {
      printf(RED "Invalid choice.\n" RESET);
      return;
    }
  }
  // Update the user data in the file
  file = fopen("choice_list.csv", "r");
  FILE *temp_file = fopen("temp.csv", "w");
  if (file == NULL || temp_file == NULL) {
    printf(RED "Error opening file.\n" RESET);
    return;
  }

  struct User temp_user;
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    sscanf(buffer, "%49[^,],%49[^,],%49[^,],%f,%49[^,],%d,%[^,],%[^,]",
           temp_user.username, temp_user.first_name, temp_user.last_name,
           &temp_user.cgpa, temp_user.department, &temp_user.semester,
           temp_user.selected_course_codes, temp_user.selected_course_names);

    if (strcmp(username, temp_user.username) == 0) {
      fprintf(temp_file, "%s,%s,%s,%f,%s,%d", user.username, user.first_name,
              user.last_name, user.cgpa, user.department, user.semester);
      for (int i = 0; i < user.num_courses; i++) {
        fprintf(temp_file, ",%s %s", user.selected_course_codes[i],
                user.selected_course_names[i]);
      }
      fprintf(temp_file, "\n");
    } else {
      fprintf(temp_file, "%s,%s,%s,%f,%s,%d,%s,%s,\n", temp_user.username,
              temp_user.first_name, temp_user.last_name, temp_user.cgpa,
              temp_user.department, temp_user.semester,
              temp_user.selected_course_names, temp_user.selected_course_codes);
    }
  }

  fclose(file);
  fclose(temp_file);

  remove("choice_list.csv");
  rename("temp.csv", "choice_list.csv");
}
  struct Student {
    char username[50];
    char first_Name[50];
    char last_Name[50];
    float cgpa;
    char department[10];
    int semester;
    char courses[50][MAX_COURSE_LENGTH];
    int numCourses;
  };

  struct Allotment {
    char username[50];
    char first_name[50];
    char last_name[50];
    float cgpa;
    char dept[10];
    char courses[50][MAX_COURSE_LENGTH];
    int numAllottedCourses;
  };

  struct Waitlist {
    char username[50];
    char first_name[50];
    char last_name[50];
    float cgpa;
    char dept[10];
    char courses[50][MAX_COURSE_LENGTH];
    int numWaitlistedCourses;
  };

  struct CourseAllotment {
    char course_code[MAX_COURSE_CODE_LEN];
    char sec_course_code[MAX_COURSE_CODE_LEN];
    char course_name[MAX_COURSE_NAME_LEN];
    int num_allotted;
    int num_waitlisted;
    int seats_left;
  };

  void save_allotments_to_file(const char *filename, struct Allotment *allotments,
                               int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
      printf("Error opening file %s for writing.\n", filename);
      return;
    }

    fprintf(file, "Username,First Name,Last Name,CGPA,Department,Courses\n");

    for (int i = 0; i < count; i++) {
      fprintf(file, "%s,%s,%s,%.2f,%s", allotments[i].username,
              allotments[i].first_name, allotments[i].last_name,
              allotments[i].cgpa, allotments[i].dept);
      for (int j = 0; j < allotments[i].numAllottedCourses; j++) {
        fprintf(file, ",%s", allotments[i].courses[j]);
      }
      fprintf(file, "\n");
    }

    fclose(file);
  }

  void save_waitlist_to_file(const char *filename, struct Waitlist *waitlist, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
      printf("Error opening file %s for writing.\n", filename);
      return;
    }

    fprintf(file, "Username,First Name,Last Name,CGPA,Department,Courses\n");

    for (int i = 0; i < count; i++) {
      fprintf(file, "%s,%s,%s,%.2f,%s", waitlist[i].username, waitlist[i].first_name, waitlist[i].last_name, waitlist[i].cgpa, waitlist[i].dept);
      for (int j = 0; j < waitlist[i].numWaitlistedCourses; j++) {
        fprintf(file, ",%s", waitlist[i].courses[j]);
      }
      fprintf(file, "\n");
    }

    fclose(file);
  }

  void append_to_course_file(const char *course, const char *username,
                             const char *first_name, const char *last_name,
                             float cgpa, const char *dept) {
    char filename[100];
    sprintf(filename, "%s_section.csv", course);

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
      printf("Error opening file %s for appending.\n", filename);
      return;
    }

    fprintf(file, "%s,%s,%s,%.2f,%s\n", username, first_name, last_name, cgpa,
            dept);
    fclose(file);
  }

  void save_course_allotments_to_file(const char *filename,
                                      struct CourseAllotment *course_allotments,
                                      int count) {
    FILE *r_file = fopen("list.csv", "r");
    if (r_file == NULL) {
      printf("Error in opening list.csv for reading.\n");
      return;
    }

    char buffer[1024];
    char department[MAX_DEPARTMENT_LEN];
    char semester[MAX_SEMESTER_LEN];
    char course_code[MAX_COURSE_CODE_LEN];
    char sec_course_code[MAX_COURSE_CODE_LEN];
    char course_type[15];
    char course_name[MAX_COURSE_NAME_LEN];
    float course_req_cgpa;

    printf("Reading list.csv for course details...\n");
    fgets(buffer, sizeof(buffer), r_file); // Skip the header line
    while (fgets(buffer, sizeof(buffer), r_file)) {
      sscanf(buffer,
             "%49[^,],%9[^,],%49[^,],%49[^,],%14[^,],%99[^,],%f,%*[^,],%*[^,],%*["
             "^,]",
             department, semester, course_code, sec_course_code, course_type,
             course_name, &course_req_cgpa);

      printf("Read course: %s, %s, %s\n", sec_course_code, course_code,
             course_name);

      for (int i = 0; i < count; i++) {
        if (strcmp(sec_course_code, course_allotments[i].course_code) == 0) {
          strcpy(course_allotments[i].sec_course_code, course_code);
          strcpy(course_allotments[i].course_name, course_name);
        }
      }
    }

    fclose(r_file);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
      printf("Error opening %s file for writing.\n", filename);
      return;
    }

    fprintf(file, "Course_code,Section_Code,Name,Number Allotted,Seats "
                  "Left,Number Waitlisted\n");
    for (int i = 0; i < count; i++) {
      fprintf(file, "%s,%s,%s,%d,%d,%d\n", course_allotments[i].course_code,
              course_allotments[i].sec_course_code, course_allotments[i].course_name,
              course_allotments[i].num_allotted, course_allotments[i].seats_left,
              course_allotments[i].num_waitlisted);

      /*printf("Saved course allotment: %s,%s,%s,%d,%d,%d\n",
             course_allotments[i].sec_course_code,
             course_allotments[i].course_code, course_allotments[i].course_name,
             course_allotments[i].num_allotted, course_allotments[i].seats_left,
             course_allotments[i].num_waitlisted);*/
    }

    fclose(file);
  }

  void allotted(const char *course_code, int num_allotted, int seats_left,
                int num_waitlisted) {
    printf("Course Code: %s\n", course_code);
    printf("Number Allotted: %d\n", num_allotted);
    printf("Seats Left: %d\n", seats_left);
    printf("Number Waitlisted: %d\n\n", num_waitlisted);
  }

  int is_valid_code(const char *code) {
    if (strlen(code) <= 1) {
      return 0;
    }
    if (isdigit(code[0])) {
      return 0;
    }
    return 1;
  }

  void allotment() {
    FILE *file = fopen("sorted_list.csv", "r");
    if (file == NULL) {
      printf("Error opening sorted_list.csv file.\n");
      return;
    }

    struct Student students[MAX_STUDENTS];
    struct CourseAllotment course_allotments[MAX_COURSES] = {0};
    int studentIndex = 0, courseAllotmentIndex = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
      char *token;
      token = strtok(line, ",");
      strcpy(students[studentIndex].username, token);

      token = strtok(NULL, ",");
      strcpy(students[studentIndex].first_Name, token);

      token = strtok(NULL, ",");
      strcpy(students[studentIndex].last_Name, token);

      token = strtok(NULL, ",");
      students[studentIndex].cgpa = atof(token);

      token = strtok(NULL, ",");
      strcpy(students[studentIndex].department, token);

      token = strtok(NULL, ",");
      students[studentIndex].semester = atoi(token);

      int courseIndex = 0;
      while ((token = strtok(NULL, ",")) != NULL) {
        strcpy(students[studentIndex].courses[courseIndex], token);
        courseIndex++;
      }
      students[studentIndex].numCourses = courseIndex;
      studentIndex++;
    }
    fclose(file);

    int max_limit;
    printf("Enter the number of maximum students per section: ");
    scanf("%d", &max_limit);
    printf("\n");

    struct Allotment allotments[MAX_STUDENTS] = {0};
    struct Waitlist waitlists[MAX_STUDENTS] = {0};
    int allotmentIndex = 0;
    int waitlistIndex = 0;
    int enrolled_count[MAX_COURSES][MAX_SECTIONS] = {0}; // Max 3 sections per course

    for (int p = 0; p < studentIndex; p++) {
      allotments[p].numAllottedCourses = 0;
      strcpy(allotments[p].username, students[p].username);
      strcpy(allotments[p].first_name, students[p].first_Name);
      strcpy(allotments[p].last_name, students[p].last_Name);
      allotments[p].cgpa = students[p].cgpa;
      strcpy(allotments[p].dept, students[p].department);

      struct Waitlist tempWaitlist = {0};
      strcpy(tempWaitlist.username, students[p].username);
      strcpy(tempWaitlist.first_name, students[p].first_Name);
      strcpy(tempWaitlist.last_name, students[p].last_Name);
      tempWaitlist.cgpa = students[p].cgpa;
      strcpy(tempWaitlist.dept, students[p].department);

      for (int i = 0; i < students[p].numCourses; i++) {
        int allocated = 0;
        for (int j = 0; j < MAX_SECTIONS; j++) {
          if (enrolled_count[i][j] < max_limit) {
            strcpy(allotments[p].courses[allotments[p].numAllottedCourses],
                   students[p].courses[i]);
            allotments[p].numAllottedCourses++;
            enrolled_count[i][j]++;
            append_to_course_file(students[p].courses[i], students[p].username,
                                  students[p].first_Name, students[p].last_Name,
                                  students[p].cgpa, students[p].department);
            allocated = 1;

            // Update or add course allotment details
            int found = 0;
            for (int k = 0; k < courseAllotmentIndex; k++) {
              if (strcmp(course_allotments[k].course_code,
                         students[p].courses[i]) == 0) {
                course_allotments[k].num_allotted++;
                course_allotments[k].seats_left--;
                found = 1;
                break;
              }
            }
            if (!found) {
              strcpy(course_allotments[courseAllotmentIndex].course_code,
                     students[p].courses[i]);
              course_allotments[courseAllotmentIndex].num_allotted = 1;
              course_allotments[courseAllotmentIndex].seats_left = max_limit - 1;
              courseAllotmentIndex++;
            }
            break;
          }
        }
        if (!allocated) {
          for (int j = 0; j < MAX_SECTIONS && (i + j) < students[p].numCourses;
               j++) {
            strcpy(tempWaitlist.courses[tempWaitlist.numWaitlistedCourses],
                   students[p].courses[i + j]);
            tempWaitlist.numWaitlistedCourses++;

            // Update or add course waitlist details
            int found = 0;
            for (int k = 0; k < courseAllotmentIndex; k++) {
              if (strcmp(course_allotments[k].course_code,
                         students[p].courses[i + j]) == 0) {
                course_allotments[k].num_waitlisted++;
                found = 1;
                break;
              }
            }
            if (!found) {
              strcpy(course_allotments[courseAllotmentIndex].course_code,
                     students[p].courses[i + j]);
              course_allotments[courseAllotmentIndex].num_allotted = 0;
              course_allotments[courseAllotmentIndex].num_waitlisted = 1;
              course_allotments[courseAllotmentIndex].seats_left = max_limit;
              courseAllotmentIndex++;
            }
          }
        }
      }

      if (allotments[p].numAllottedCourses > 0) {
        allotmentIndex++;
      }

      if (tempWaitlist.numWaitlistedCourses > 0) {
        waitlists[waitlistIndex] = tempWaitlist;
        waitlistIndex++;
      }
    }

    // Save allotments to CSV file
    save_allotments_to_file("Allotment.csv", allotments, allotmentIndex);

    // Save waitlist to CSV file
    save_waitlist_to_file("Waitlist.csv", waitlists, waitlistIndex);

    // Save course allotment details to CSV file
    save_course_allotments_to_file("CourseAllotments.csv", course_allotments,
                                   courseAllotmentIndex);

    // Call the allotted function for each course
    for (int i = 0; i < courseAllotmentIndex; i++) {
      allotted(course_allotments[i].course_code,
               course_allotments[i].num_allotted, course_allotments[i].seats_left,
               course_allotments[i].num_waitlisted);
    }
  }

  void display_result(const char *username) {
    FILE *file = fopen("Allotment.csv", "r");
    if (file == NULL) {
      printf("Error in opening the Allotment.csv file.\n");
      return;
    }

    char user[50];
    char first_name[50];
    char last_name[99];
    char course[200];
    char allot_courses[MAX_COURSES][100];

    int found = 0;

    while (fscanf(file, "%49[^,],%49[^,],%99[^,],%199[^\n]\n", user, first_name,
                  last_name, course) != EOF) {
      if (strcmp(user, username) == 0) {
        int k = 0;
        char *token = strtok(course, ",");
        while (token != NULL && k < MAX_COURSES) {
          strcpy(allot_courses[k], token);
          k++;
          token = strtok(NULL, ",");
        }
        printf("Allotted courses for %s %s:\n", first_name, last_name);
        for (int l = 0; l < k; l++) {
          printf("%s\n", allot_courses[l]);
        }
        found = 1;
        break;
      }
    }

    fclose(file);

    if (!found) {
      FILE *w_file = fopen("Waitlist.csv", "r");
      if (w_file == NULL) {
        printf("Error in opening the Waitlist.csv file.\n");
        return;
      }

      while (fscanf(w_file, "%49[^,],%49[^,],%99[^,],%199[^\n]\n", user,
                    first_name, last_name, course) != EOF) {
        if (strcmp(user, username) == 0) {
          int k = 0;
          char *token = strtok(course, ",");
          while (token != NULL && k < MAX_COURSES) {
            strcpy(allot_courses[k], token);
            k++;
            token = strtok(NULL, ",");
          }
          printf("Waitlisted courses for %s %s:\n", first_name, last_name);
          for (int l = 0; l < k; l++) {
            printf("%s\n", allot_courses[l]);
          }
          break;
        }
      }

      fclose(w_file);
    }
  }

  void del_course_admin() {
    FILE *file = fopen("list.csv", "r");
    if (file == NULL) {
      printf("Error opening the file for deletion.\n");
      return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
      printf("Error opening temporary file.\n");
      fclose(file);
      return;
    }

    char course_code[MAX_COURSE_CODE_LEN];
    char course_name[MAX_COURSE_NAME_LEN];
    char department[MAX_DEPARTMENT_LEN];
    char semester[MAX_SEMESTER_LEN];
    char buffer[1024];
    char unique_course_code[MAX_COURSE_CODE_LEN];
    char course_type[15];
    float course_req_cgpa;
    char course_details[1024];

    // Read and write the header line
    if (fgets(buffer, sizeof(buffer), file)) {
      fputs(buffer, temp);
    }

    int found = 0;
    int sem;
    char dept[10];
    printf("Enter the Semester for which the course is to be deleted: ");
    scanf("%d", &sem);
    printf("\n");
    printf("Enter the department as CSE,ME,CE,EE: ");
    scanf("%s", dept);
    printf("\n");
    printf("COURSE CODE SECTION CODE COURSE NAME\n");

    // Display relevant courses
    while (fgets(buffer, sizeof(buffer), file)) {
      sscanf(buffer,
             "%49[^,],%9[^,],%19[^,],%49[^,],%14[^,],%99[^,],%f,%1023[^\n]",
             department, semester, unique_course_code, course_code, course_type,
             course_name, &course_req_cgpa, course_details);

      if (sem == atoi(semester) && strcmp(department, dept) == 0) {
        printf("%s      %s      %s\n", unique_course_code, course_code,
               course_name);
      }
    }

    // Prompt for the course code to delete
    printf("Enter the course code to delete: ");
    char del_course[MAX_COURSE_CODE_LEN];
    scanf("%49s", del_course);

    // Reset the file pointer to the beginning
    fseek(file, 0, SEEK_SET);
    fgets(buffer, sizeof(buffer), file); // Skip header again

    // Re-process file and write to temp
    while (fgets(buffer, sizeof(buffer), file)) {
      sscanf(buffer,
             "%49[^,],%9[^,],%19[^,],%49[^,],%14[^,],%99[^,],%f,%1023[^\n]",
             department, semester, unique_course_code, course_code, course_type,
             course_name, &course_req_cgpa, course_details);

      if (strcmp(unique_course_code, del_course) == 0) {
        found = 1;
        continue; // Skip writing this line to the temp file
      }
      fputs(buffer, temp);
    }

    fclose(file);
    fclose(temp);

    if (found) {
      remove("list.csv");
      rename("temp.csv", "list.csv");
      printf("Course deleted successfully.\n");
    } else {
      remove("temp.csv");
      printf("Course code not found.\n");
    }
  }

  void add_course_admin() {
    FILE *file = fopen("list.csv", "r");
    if (file == NULL) {
      printf("Error opening the file for addition of course by the admin.\n");
      return;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL) {
      printf("Error opening the temporary file.\n");
      fclose(file);
      return;
    }

    // Copying the content of list.csv to temp.csv
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
      fputs(buffer, temp);
    }

    // Close the original file as it is no longer needed
    fclose(file);

    // Prompt user for new course details
    printf("Enter the semester in which a new course is to be added: ");
    int sem;
    scanf("%d", &sem);
    printf("\n");

    char dept[MAX_DEPARTMENT_LEN];
    printf("Enter the department (CSE, CE, ME, EE): ");
    scanf("%s", dept);
    printf("\n");

    printf("Please fill the following details for the course to be added:\n");
    char new_sec_course_code[MAX_COURSE_CODE_LEN];
    char n_course_name[MAX_COURSE_NAME_LEN];
    char new_course_code[MAX_COURSE_CODE_LEN];
    char new_course_type[15];
    float new_course_req_cgpa;
    char new_faculty_name[MAX_COURSE_NAME_LEN];

    printf("Enter the course code: ");
    scanf("%s", new_course_code);
    printf("Enter the course section code: ");
    scanf("%s", new_sec_course_code);
    printf("Enter the course name: ");
    scanf("%s", n_course_name);
    printf("Enter the course type (Core or Elective): ");
    scanf("%s", new_course_type);
    printf("Enter the minimum CGPA required to enroll for the course: ");
    scanf("%f", &new_course_req_cgpa);
    printf("Enter the credit for course: ");
    float credit;
    scanf("%f", &credit);
    printf("Enter the faculty name for the course: ");
    scanf("%s", new_faculty_name);

    // Write the new course details to the temporary file
    fprintf(temp, "%s,%d,%s,%s,%s,%s,%.2f,%.2f,%s\n", dept, sem, new_course_code,
            new_sec_course_code, new_course_type, n_course_name,
            new_course_req_cgpa, credit, new_faculty_name);

    fclose(temp);

    // Replace the original file with the updated temporary file
    remove("list.csv");
    rename("temp.csv", "list.csv");

    printf("Course added successfully.\n");
  }

  struct Faculty {
    char name[MAX_FIELD_LEN];
    char degree[MAX_FIELD_LEN];
    char college[MAX_FIELD_LEN];
    char email[MAX_FIELD_LEN];
    int experience;
  };

  void search_faculty() {
    char first_name[MAX_FIELD_LEN];
    char last_name[MAX_FIELD_LEN];

    printf("Enter faculty first name: ");
    scanf("%s", first_name);
    printf("Enter faculty last name: ");
    scanf("%s", last_name);
    FILE *file = fopen("faculty_list.csv", "r");
    if (file == NULL) {
      printf("Error opening file.\n");
      return;
    }

    struct Faculty faculty;
    int found = 0;

    while (fscanf(file, "%[^,],%[^,],%[^,],%[^,],%d\n", faculty.name,
                  faculty.degree, faculty.college, faculty.email,
                  &faculty.experience) != EOF) {

      if (strstr(faculty.name, first_name) && strstr(faculty.name, last_name)) {
        found = 1;
        printf("Faculty Details:\n");
        printf("\n");
        printf("Name: %s\n", faculty.name);
        printf("Graduation Degree: %s\n", faculty.degree);
        printf("Graduation College: %s\n", faculty.college);
        printf("Email Address: %s\n", faculty.email);
        printf("Years of Experience: %d\n", faculty.experience);
        break;
      }
    }

    if (!found) {
      printf("Faculty not found in the list.\n");
    }

    fclose(file);
  }

  int is_student_in_waitlist(const char *username, const char *waitlist_file) {
    FILE *file = fopen(waitlist_file, "r");
    if (!file) {
      fprintf(stderr, "Error: %s file not found.\n", waitlist_file);
      return 0;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
      line[strcspn(line, "\n")] = 0; // Remove newline character
      char waitlisted_username[MAX_FIELD_LEN];
      sscanf(line, "%[^,]", waitlisted_username);
      if (strcmp(waitlisted_username, username) == 0) {
        fclose(file);
        return 1;
      }
    }
    fclose(file);
    return 0;
  }

  int get_user_input(const char *question) {
    char response[4]; // Buffer to hold "yes" or "no"
    while (1) {
      printf("%s (yes/no): ", question);
      scanf("%3s", response);
      if (strcmp(response, "yes") == 0) {
        return 1;
      } else if (strcmp(response, "no") == 0) {
        return 0;
      } else {
        printf("Please answer with 'yes' or 'no'.\n");
      }
    }
  }

  int calculate_score(int responses[], int weights[], int num_questions) {
    int score = 0;
    for (int i = 0; i < num_questions; i++) {
      if (responses[i]) {
        score += weights[i];
      }
    }
    return score;
  }
  #define MAX_FIELD_LEN 100
  #define MAX_LINE_LEN 500

  void waitlist_final(const char *username) {
    // Define questions and their corresponding weights
    const char *questions[] = {"Have you done an internship?",
                               "Have you qualified any national level olympiad?",
                               "Have you published any research papers?",
                               "Have you participated in any major project?",
                               "Have you received any significant awards?"};
    int weights[] = {20, 30, 25, 15, 10}; // Example weightages for each question
    int num_questions = sizeof(weights) / sizeof(weights[0]);

    FILE *input_file = fopen("Waitlist.csv", "r");
    if (!input_file) {
      fprintf(stderr, "Error: Waitlist.csv file not found.\n");
      return;
    }

    FILE *output_file = fopen("updated_waitlisted.csv", "a+");
    if (!output_file) {
      fprintf(stderr,
              "Error: could not open updated_waitlisted.csv for writing.\n");
      fclose(input_file);
      return;
    }

    // Check if the file is empty and write the header if it is
    fseek(output_file, 0, SEEK_END);
    long size = ftell(output_file);
    if (size == 0) {
      fprintf(output_file,
              "Username,First Name,Last Name,CGPA,Department,Score,Courses\n");
    }

    // Read and process each student in the waitlist
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), input_file)) {
      line[strcspn(line, "\n")] = 0; // Remove newline character
      char file_username[MAX_FIELD_LEN], first_name[MAX_FIELD_LEN],
          last_name[MAX_FIELD_LEN];
      char cgpa[MAX_FIELD_LEN], department[MAX_FIELD_LEN], courses[MAX_LINE_LEN];

      // Assume a CSV structure of Username,First Name,Last
      // Name,CGPA,Department,Courses
      sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]", file_username,
             first_name, last_name, cgpa, department, courses);

      // Only process if the current username matches the input username
      if (strcmp(file_username, username) == 0) {
        printf("Processing student: %s\n", username);

        // Collect responses and calculate score for the student
        int responses[num_questions];
        for (int i = 0; i < num_questions; i++) {
          responses[i] = get_user_input(questions[i]);
        }
        int score = calculate_score(responses, weights, num_questions);

        // Write the student data along with the calculated score to the output
        // file
        fprintf(output_file, "%s,%s,%s,%s,%s,%d,%s\n", file_username, first_name,
                last_name, cgpa, department, score, courses);

        // Exit after processing the student
        break;
      }
    }

    fclose(input_file);
    fclose(output_file);
  }

  int is_student_in_allotedlist(const char *username, const char *alloted_file) {
    FILE *file = fopen(alloted_file, "r");
    if (!file) {
      fprintf(stderr, "Error: %s file not found.\n", alloted_file);
      return 0;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
      line[strcspn(line, "\n")] = 0; // Remove newline character
      char alloted_username[MAX_FIELD_LEN];
      sscanf(line, "%[^,]", alloted_username);
      if (strcmp(alloted_username, username) == 0) {
        fclose(file);
        return 1;
      }
    }
    fclose(file);
    return 0;
  }

  struct Student1 {
    char username[MAX_FIELD_LEN];
    char first_name[MAX_FIELD_LEN];
    char last_name[MAX_FIELD_LEN];
    float cgpa;
    char department[MAX_FIELD_LEN];
    int score;
    char courses[MAX_LINE_LEN];
  };

  // Comparison function for qsort
  int compare_students(const void *a, const void *b) {
    const struct Student1 *student1 = (const struct Student1 *)a;
    const struct Student1 *student2 = (const struct Student1 *)b;
    // Compare scores in descending order
    return student2->score - student1->score;
  }

  void rank_students() {
    // Open input and output files
    FILE *input_file = fopen("updated_waitlisted.csv", "r");
    if (!input_file) {
      fprintf(stderr, "Error: Unable to open input file.\n");
      return;
    }

    FILE *output_file = fopen("updated_sorted.csv", "w");
    if (!output_file) {
      fprintf(stderr, "Error: Unable to create output file.\n");
      fclose(input_file);
      return;
    }

    // Read and ignore the header line
    char header[MAX_LINE_LEN];
    if (fgets(header, sizeof(header), input_file) == NULL) {
      fprintf(stderr, "Error: Failed to read header line.\n");
      fclose(input_file);
      fclose(output_file);
      return;
    }

    // Read students' data and store them in an array of structs
    struct Student1 students[100]; // Assuming there are at most 100 students
    int num_students = 0;

    while (fgets(header, sizeof(header), input_file)) {
      sscanf(header, "%[^,],%[^,],%[^,],%f,%[^,],%d,%[^\n]",
             students[num_students].username, students[num_students].first_name,
             students[num_students].last_name, &students[num_students].cgpa,
             students[num_students].department, &students[num_students].score,
             students[num_students].courses);
      num_students++;
    }

    // Sort students based on their scores
    qsort(students, num_students, sizeof(struct Student1), compare_students);

    // Write sorted students' data to the output file
    fprintf(output_file,
            "Username,First Name,Last Name,CGPA,Department,Score,Courses\n");
    for (int i = 0; i < num_students; i++) {
      fprintf(output_file, "%s,%s,%s,%.2f,%s,%d,%s\n", students[i].username,
              students[i].first_name, students[i].last_name, students[i].cgpa,
              students[i].department, students[i].score, students[i].courses);
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    // Remove the original file
    remove("updated_waitlist.csv");
  }

  struct Student2 {
    char username[50];
    char first_Name[50];
    char last_Name[50];
    float cgpa;
    char department[10];
    int score;
    char courses[20][MAX_COURSE_CODE_LEN];
    int numCourses;
  };

  void reallocate() {
    FILE *file = fopen("updated_sorted.csv", "r");
    if (file == NULL) {
      printf("Error opening updated_sorted.csv file.\n");
      return;
    }
    correct_file();

    struct Student2 waitlisted_students[MAX_STUDENTS];
    struct CourseAllotment course_allotments[MAX_COURSES] = {0};
    int studentIndex = 0, courseAllotmentIndex = 0;
    char line[1024];

    // Read student data from updated_sorted.csv
    fgets(line, sizeof(line), file); // Skip header
    while (fgets(line, sizeof(line), file)) {
      char *token;
      token = strtok(line, ",");
      strcpy(waitlisted_students[studentIndex].username, token);

      token = strtok(NULL, ",");
      strcpy(waitlisted_students[studentIndex].first_Name, token);

      token = strtok(NULL, ",");
      strcpy(waitlisted_students[studentIndex].last_Name, token);

      token = strtok(NULL, ",");
      waitlisted_students[studentIndex].cgpa = atof(token);

      token = strtok(NULL, ",");
      strcpy(waitlisted_students[studentIndex].department, token);

      token = strtok(NULL, ",");
      waitlisted_students[studentIndex].score = atoi(token);

      int courseIndex = 0;
      while ((token = strtok(NULL, ",")) != NULL) {
        strcpy(waitlisted_students[studentIndex].courses[courseIndex], token);
        courseIndex++;
      }
      waitlisted_students[studentIndex].numCourses = courseIndex;
      studentIndex++;
    }
    fclose(file);

    // Ask admin for the number by which to update the section max_limit
    int additional_limit;
    printf("Enter the number by which the section max_limit should be updated: ");
    scanf("%d", &additional_limit);

    // Load current course allotment data (example file: CourseAllotments.csv)
    FILE *allotment_file = fopen("CourseAllotments.csv", "r");
    if (allotment_file == NULL) {
      printf("Error opening CourseAllotments.csv file.\n");
      return;
    }

    // Read course allotment data from CourseAllotments.csv
    fgets(line, sizeof(line), allotment_file); // Skip header
    while (fgets(line, sizeof(line), allotment_file)) {
      char *token;
      token = strtok(line, ",");
      strcpy(course_allotments[courseAllotmentIndex].course_code, token);

      token = strtok(NULL, ",");
      strcpy(course_allotments[courseAllotmentIndex].sec_course_code, token);

      token = strtok(NULL, ",");
      strcpy(course_allotments[courseAllotmentIndex].course_name, token);

      token = strtok(NULL, ",");
      course_allotments[courseAllotmentIndex].num_allotted = atoi(token);

      token = strtok(NULL, ",");
      course_allotments[courseAllotmentIndex].seats_left = atoi(token);

      token = strtok(NULL, ",");
      course_allotments[courseAllotmentIndex].num_waitlisted = atoi(token);

      course_allotments[courseAllotmentIndex].seats_left += additional_limit; // Update seats_left
      courseAllotmentIndex++;
    }
    fclose(allotment_file);

    // Reallocate students from updated_sorted.csv
    for (int i = 0; i < studentIndex; i++) {
      for (int j = 0; j < waitlisted_students[i].numCourses; j++) {
        for (int k = 0; k < courseAllotmentIndex; k++) {
          if (strcmp(waitlisted_students[i].courses[j],
                     course_allotments[k].course_code) == 0 &&
              course_allotments[k].seats_left > 0) {
            course_allotments[k].num_allotted++;
            course_allotments[k].seats_left--;
            printf("Allocated course %s to student %s\n",
                   waitlisted_students[i].courses[j],
                   waitlisted_students[i].username);

            // Remove student from waitlist by shifting remaining students up
            for (int l = i; l < studentIndex - 1; l++) {
              waitlisted_students[l] = waitlisted_students[l + 1];
            }
            studentIndex--; // Decrease student count
            i--;            // Check the same index again after shifting
            break;
          }
        }
      }
    }

    // Save updated course allotment data to CSV file
    FILE *updated_allotment_file = fopen("Updated_CourseAllotments.csv", "w");
    if (updated_allotment_file == NULL) {
      printf("Error opening Updated_CourseAllotments.csv file.\n");
      return;
    }
    fprintf(updated_allotment_file,
            "Course Code,Section Course Code,Course Name,Num Allotted,Seats Left,Num Waitlisted\n");
    for (int i = 0; i < courseAllotmentIndex; i++) {
      fprintf(
          updated_allotment_file, "%s,%s,%s,%d,%d,%d\n",
          course_allotments[i].course_code, course_allotments[i].sec_course_code,
          course_allotments[i].course_name, course_allotments[i].num_allotted,
          course_allotments[i].seats_left, course_allotments[i].num_waitlisted);
    }
    fclose(updated_allotment_file);
  }
  /*int is_valid_code(const char *code) {
      if (strlen(code) <= 1) {
          return 0;
      }
      if (isdigit(code[0])) {
          return 0;
      }
      return 1;
  }*/

  void copy_filtered_course_allotments(const char *filename) {
      char temp_filename[] = "temp_CourseAllotments.csv";

      FILE *input_file = fopen(filename, "r");
      FILE *output_file = fopen(temp_filename, "w");

      if (input_file == NULL) {
          printf("Error opening file %s for reading.\n", filename);
          return;
      }
      if (output_file == NULL) {
          printf("Error opening temporary file for writing.\n");
          fclose(input_file);
          return;
      }

      char line[MAX_LINE_LENGTH];
      int line_count = 0;

      while (fgets(line, sizeof(line), input_file)) {
          line_count++;

          // Skip the header line
          if (line_count == 1) {
              continue;
          }

          // Check if the line is a gap
          int comma_count = 0;
          for (int i = 0; line[i] != '\0'; i++) {
              if (line[i] == ',') {
                  comma_count++;
              }
          }

          // Skip lines with gaps
          if (comma_count < 5) {
              continue;
          }

          // Split the line by commas and extract the required fields
          char *token;
          char course_code[MAX_LINE_LENGTH] = "";
          int num_allotted = 0;
          int seats_left = 0;
          int num_waitlisted = 0;

          // Tokenize the line
        token = strtok(line, ",");
        if (token != NULL) {
            if (is_valid_code(token)) {
                strcpy(course_code, token);
            } else {
                continue; // Skip this line if the sec_course_code is not valid
            }
        }

          for (int i = 0; i < 4; i++) {
              token = strtok(NULL, ",");
              if (token == NULL) break;

              if (i == 1) {
                  num_allotted = atoi(token);
              } else if (i == 2) {
                  seats_left = atoi(token);
              } else if (i == 3) {
                  num_waitlisted = atoi(token);
              }
          }

          // Write the required fields to the temporary output file
          fprintf(output_file, "%s,%d,%d,%d\n", course_code, num_allotted, seats_left, num_waitlisted);
      }

      fclose(input_file);
      fclose(output_file);

      // Replace the original file with the temporary file
      if (remove(filename) != 0) {
          printf("Error removing original file %s.\n", filename);
          return;
      }

      if (rename(temp_filename, filename) != 0) {
          printf("Error renaming temporary file to %s.\n", filename);
      }
  }

  int correct_file() {
      const char *filename = "CourseAllotments.csv";
      copy_filtered_course_allotments(filename);
      return 0;
  }
