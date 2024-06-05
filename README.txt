Student Information Website Generator
This C++ program generates a simple website that displays information about students from a CSV file. Each student's information is presented on a separate HTML page within the website.

Features
Reads student information from a CSV file.
Creates a directory structure for the website.
Moves image files to the appropriate directory.
Generates CSS file for styling.
Allows user to select which information to include on the HTML pages.
Generates HTML pages for each student.

Prerequisites
Windows operating system.
Visual C++17 compiler (tested with Visual Studio).
Basic knowledge of C++ programming language.

How to Use
Compile the Program: Compile the provided source code using a C++ compiler.
Prepare Input Files:
Prepare a CSV file containing student information. The CSV file should have the following format: "ID","Name","Department","Batch","DOB","Image","Description","Hobby1","Hobby2",...
Prepare a directory containing images referenced in the CSV file.
Run the Program:
Execute the compiled program.
Enter the path to the base directory for the website.
Enter the path to the directory containing the images.
Customize Output:
Choose which information to include on the HTML pages by entering '1' or '0' for each type of information.
View Output:
Once the program finishes execution, navigate to the website directory to view the generated HTML files.
Example
Suppose you have a CSV file named students.csv containing student information and an image directory named images. After compiling and running the program, you choose to include the student's name, ID, department, and hobby on the HTML pages.

The program will generate a website directory structure as follows:
Website/
│
├── Images/
│   ├── image1.jpg
│   ├── image2.jpg
│   └── ...
│
├── personal.css
│
├── student1.html
├── student2.html
└── ...
Notes
Ensure that the CSV file is properly formatted with the required information.
Images referenced in the CSV file should be present in the specified directory.