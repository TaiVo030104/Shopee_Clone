#include"Header.h"

int main()
{
    // Đặt chế độ mode cho stdin và stdout để hỗ trợ đọc và ghi văn bản Unicode
    [[maybe_unused]] auto modeOut = _setmode(_fileno(stdout), _O_WTEXT);
    [[maybe_unused]] auto modeIn = _setmode(_fileno(stdin), _O_WTEXT);

    // Nhập đường dẫn thư mục gốc cho website
    wchar_t basepath[MAX_PATH];
    wprintf(L"Nhập đường dẫn thư mục Website: ");
    fgetws(basepath, MAX_PATH, stdin);
    basepath[wcslen(basepath) - 1] = L'\0'; // Xóa ký tự newline

    // Tạo thư mục cho website và cấu hình file HTML
    wchar_t websitename[] = L"Website"; // Tên mặc định cho website
    createWebsiteDirectory(basepath, websitename);

    // Nhập đường dẫn thư mục chứa hình ảnh
    wchar_t imagesPath[MAX_PATH];
    wprintf(L"Nhập đường dẫn thư mục images: ");
    fgetws(imagesPath, MAX_PATH, stdin);
    imagesPath[wcslen(imagesPath) - 1] = L'\0'; // Xóa ký tự newline

    // Di chuyển hình ảnh vào thư mục của website
    wchar_t websiteDirectory[MAX_PATH];
    swprintf(websiteDirectory, MAX_PATH, L"%s\\%s", basepath, websitename);
    moveImagesFolder(imagesPath, websiteDirectory);

    // Tạo và in file CSS
    wchar_t cssFile[] = L"personal.css";
    wchar_t cssFilePath[MAX_PATH];
    swprintf(cssFilePath, MAX_PATH, L"%s\\%s", websiteDirectory, cssFile); // Tạo đường dẫn cho tệp tin CSS
    createAndPrintCSSFile(cssFilePath, cssFile);

    // Đọc dữ liệu từ tệp CSV
    wchar_t filename[] = L"sinhvien.csv";
    int numStudents = 0;
    Student* students = readFileAndAllocateMemory(filename, numStudents);

    // Tạo đường dẫn thư mục chứa hình ảnh trong website
    wchar_t imageDirectory[MAX_PATH];
    swprintf(imageDirectory, MAX_PATH, L"%s\\Images", websiteDirectory);

    int numSelectedInfo;
    int* selectedInfo = inputSelectedInfo(numSelectedInfo);

    // Tạo file HTML cho mỗi sinh viên
    createHTMLFiles(students, numStudents, websiteDirectory, imageDirectory, selectedInfo);

    // Giải phóng bộ nhớ đã được cấp phát cho mảng sinh viên
    for (int i = 0; i < numStudents; ++i) {
        delete[] students[i].id;
        delete[] students[i].name;
        delete[] students[i].department;
        delete[] students[i].batch;
        delete[] students[i].dob;
        delete[] students[i].image;
        delete[] students[i].description;
    }
    delete[] selectedInfo; // Giải phóng bộ nhớ của mảng lựa chọn
    delete[] students;

    return 0;
}
