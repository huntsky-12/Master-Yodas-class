# Master Yoda's Class - Course Recommendation & Allotment System

A C-based terminal project that simulates a university-level course recommendation, choice filling, and allotment system, tailored by CGPA and student interests — complete with waitlisting, faculty lookup, and eligibility filters.

---

## Features

- User authentication with credentials stored securely in `data.csv`
- Personalized course recommendations based on student CGPA and selected interests
- Choice filling for up to 6 courses with preference ranking
- Faculty inquiry system to explore instructor details
- Allotment system with handling for waitlist and final course confirmations
- Eligibility checks for department, semester, and minimum CGPA
- CSV-based storage: lightweight and file-driven data persistence

---

## File Structure

| File                   | Purpose |
|------------------------|---------|
| `main.c`               | Entry point of the application |
| `list.csv`             | Master list of all available courses |
| `data.csv`             | Registered user details |
| `choice_list.csv`      | Stores student choices and preferences |
| `Allotment.csv`        | Final allotted courses |
| `Waitlist.csv`         | Waitlisted students |
| `faculty_list.csv`     | Faculty information for lookup |
| `color.c`              | ANSI color-coded printing (UX improvements) |
| `CourseAllotments.csv` | Raw mapping of course choices and students |

---

## Technologies Used

- Language: C
- Compilation: GCC / Clang (`gcc main.c -o yoda`)
- Storage: CSV files for persistent state
- Terminal UI: ANSI escape codes for styling

---

## How to Run

```bash
git clone https://github.com/huntsky-12/Master-Yodas-class.git
cd Master-Yodas-class
gcc main.c -o yoda
./yoda
````

## User Roles

### Student

- Register and Login
- View eligible courses based on:
  - CGPA
  - Department
  - Semester
- Select up to 6 courses with 3 preferences each
- Get allotment results and interact with waitlist

### Admin (Future Scope)

- View overall student selections
- Modify allotment rules and seat availability
- View faculty/course analytics

---

## Concepts Covered

- File I/O and string parsing in C
- Multi-criteria filtering (Department + Semester + CGPA)
- CLI UX using colors and prompts
- Basic data validation
- Modular design using structs and function decomposition
- Dynamic choice filling and course preference logic

---

## Screenshots




```markdown
![Recommendation Screenshot](screenshots/recommendation.png)
![Choice Filling Screenshot](screenshots/choice_filling.png)


```
## Future Improvements
- Implement password hashing and encryption (e.g., `sha256` or `bcrypt`) for enhanced security
- Develop an admin dashboard for analytics, allotment control, and real-time seat tracking
- Add a GUI frontend using Tkinter, React, or ncurses for better usability
- Integrate basic NLP to cluster student interests for smarter recommendations
- Enable persistent session handling across logins
- Expand to a mobile or full-stack web application for broader accessibility
## Author

**HuntSky**  
Email: [huntdev12@gmail.com](mailto:huntdev12@gmail.com)  

