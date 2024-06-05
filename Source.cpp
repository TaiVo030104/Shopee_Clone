#include "Header.h"

wchar_t* removeElement(const wchar_t* str) {
    wchar_t* result = new wchar_t[wcslen(str) + 1];
    size_t index = 0;

    // Loại bỏ dấu ngoặc kép và khoảng trắng ở đầu
    while (*str && (*str == L'"' || iswspace(*str))) {
        ++str;
    }

    // Copy chuỗi vào result
    while (*str && *str != L'"') {
        result[index++] = *str++;
    }
    result[index] = L'\0';

    // Loại bỏ khoảng trắng ở cuối
    while (index > 0 && iswspace(result[index - 1])) {
        result[--index] = L'\0';
    }

    return result;
}

Student* readFileAndAllocateMemory(const wchar_t* filename, int& numStudents) {
    wifstream file(filename);
    if (!file.is_open()) {
        wcerr << L"Không thể mở file." << endl;
        numStudents = 0;
        return nullptr;
    }

    file.imbue(locale(file.getloc(), new codecvt_utf8_utf16<wchar_t>));

    wstring line;
    int count = 0;

    // Đếm số lượng sinh viên trong file để cấp phát đúng kích thước mảng
    while (getline(file, line)) {
        ++count;
    }

    file.clear(); // Đặt lại trạng thái của file
    file.seekg(0, ios::beg); // Di chuyển con trỏ đọc về đầu file

    // Cấp phát mảng để chứa thông tin sinh viên
    Student* students = new Student[count];

    // Đọc từng dòng trong file và lưu thông tin vào mảng students
    int index = 0;
    while (getline(file, line)) {
        wstringstream ss(line);
        Student& student = students[index];

        wchar_t data[256];
        ss.getline(data, 256, L',');
        student.id = removeElement(data);

        ss.getline(data, 256, L',');
        student.name = removeElement(data);

        ss.getline(data, 256, L',');
        student.department = removeElement(data);

        ss.getline(data, 256, L',');
        student.batch = removeElement(data);

        ss.getline(data, 256, L',');
        student.dob = removeElement(data);

        ss.getline(data, 256, L',');
        student.image = removeElement(data);

        ss.getline(data, 256, L',');
        student.description = removeElement(data);

        // Đọc dữ liệu cho trường hobby
        std::vector<std::wstring> hobbies;
        std::wstring hobbyData;
        wchar_t ch;
        bool inQuotes = false;
        while (ss.get(ch)) {
            if (ch == L'"') {
                inQuotes = !inQuotes;
            }
            else if (ch == L',' && !inQuotes) {
                // Kiểm tra xem hobbyData có rỗng hoặc chỉ chứa khoảng trắng không
                if (!hobbyData.empty() && !std::all_of(hobbyData.begin(), hobbyData.end(), iswspace)) {
                    hobbies.push_back(hobbyData);
                }
                hobbyData.clear();
            }
            else {
                hobbyData += ch;
            }
        }
        // Kiểm tra xem hobbyData có rỗng hoặc chỉ chứa khoảng trắng không sau khi kết thúc vòng lặp
        if (!hobbyData.empty() && !std::all_of(hobbyData.begin(), hobbyData.end(), iswspace)) {
            hobbies.push_back(hobbyData);
        }
        student.hobby = hobbies;



        // Bỏ qua kí tự đóng ngoặc kép

        ++index;
    }

    // Đóng file
    file.close();

    // Gán số lượng sinh viên vào tham số đầu ra
    numStudents = count;

    // Trả về con trỏ tới mảng chứa thông tin sinh viên
    return students;
}


void createWebsiteDirectory(const wchar_t* basePath, const wchar_t* websiteName) {
    // Tạo đường dẫn thư mục website
    wchar_t websitePath[MAX_PATH];
    swprintf(websitePath, MAX_PATH, L"%s\\%s", basePath, websiteName);

    // Tạo thư mục
    if (_wmkdir(websitePath) == 0) {
        wcout << L"Thư mục đã được tạo!" << endl;
    }
    else {
        wcerr << L"Lỗi tạo thư mục." << endl;
    }
}


// Tạo file css
void createAndPrintCSSFile(const wchar_t* directoryPath, const wchar_t* filename) {
    // Mở tệp tin để đọc
    FILE* inputFile = _wfopen(filename, L"r");
    if (!inputFile) {
        wcerr << L"Không thể mở tập tin để đọc" << endl;
        return;
    }

    // Tạo buffer để đọc nội dung từ tệp tin
    const int bufferSize = 1024;
    wchar_t buffer[bufferSize];

    // Đọc nội dung từ tệp tin
    wstring cssContent;
    while (fgetws(buffer, bufferSize, inputFile)) {
        cssContent += buffer;
    }

    // Đóng tệp tin sau khi đọc xong
    fclose(inputFile);

    // Mở tệp tin để ghi
    FILE* cssFile = _wfopen(directoryPath, L"w");
    if (!cssFile) {
        wcerr << L"Không thể tạo tệp tin để ghi" << endl;
        return;
    }

    // Ghi nội dung vào tệp tin
    fputws(cssContent.c_str(), cssFile);

    // Đóng tệp tin sau khi ghi xong
    fclose(cssFile);

    // Thông báo cho người dùng biết rằng tệp tin đã được tạo và ghi thành công
    wcout << L"Tạo thành công file CSS." << endl;
}

void moveImagesFolder(const wchar_t* sourcePath, const wchar_t* destinationPath) {
    // Kiểm tra thư mục nguồn và thư mục đích có tồn tại không
    if (!filesystem::exists(sourcePath) || !filesystem::is_directory(sourcePath)) {
        wcerr << L"Thư mục nguồn không tồn tại." << endl;
        return;
    }

    if (!filesystem::exists(destinationPath) || !filesystem::is_directory(destinationPath)) {
        wcerr << L"Thư mục đích không tồn tại." << endl;
        return;
    }

    // Tạo đường dẫn cho thư mục mới trong thư mục đích để chứa hình ảnh
    const wchar_t* newSourcePath = L"\\Images";
    std::wstring fullPath = std::wstring(destinationPath) + newSourcePath;

    if (!filesystem::exists(fullPath)) {
        filesystem::create_directory(fullPath);
    }

    // Lặp qua tất cả các tệp trong thư mục nguồn
    for (const auto& entry : filesystem::directory_iterator(sourcePath)) {
        // Tạo đường dẫn đến tệp nguồn và đích
        const std::wstring filePath = entry.path().wstring();
        const std::wstring fileName = entry.path().filename().wstring();
        std::wstring newFilePath = fullPath + L"\\" + fileName;

        // Di chuyển tệp
        if (CopyFile(filePath.c_str(), newFilePath.c_str(), FALSE)) {
            wcout << L"Đã chuyển " << filePath << L" sang " << newFilePath << endl;
        }
        else {
            wcerr << L"Có lỗi khi di chuyển " << filePath << endl;
        }

    }

    wcout << L"Đã hoàn thành di chuyển mục Images." << endl;
}

int* inputSelectedInfo(int& numSelectedInfo) {
    const int maxInfo = 7;
    int* selections = new int[maxInfo];

    std::wcout << L"Nhập các thông tin muốn in ra:\n";
    std::wcout << L"1. Họ tên\n";
    std::wcout << L"2. MSSV\n";
    std::wcout << L"3. Khoa\n";
    std::wcout << L"4. Ngày sinh\n";
    std::wcout << L"5. Email\n";
    std::wcout << L"6. Sở thích\n";
    std::wcout << L"7. Mô tả\n";

    int count = 0;
    while (true) {
        std::wcout << L"Nhập '1' để in hoặc '0' để không in: ";
        std::wstring input;
        std::getline(std::wcin, input);
        if (input.empty()) {
            break;
        }
        int choice = std::stoi(input);
        if (choice != 0 && choice != 1) {
            std::wcout << L"Lựa chọn không hợp lệ. Vui lòng nhập lại.\n";
            continue;
        }
        selections[count] = choice;
        ++count;
        if (count == maxInfo) {
            std::wcout << L"Số lượng thông tin đã đạt đến giới hạn tối đa.\n";
            break;
        }
    }
    numSelectedInfo = count;
    return selections;
}


// tạo file html
void createHTMLFiles(Student* students, int numStudents, const wchar_t* outputDirectory,  const wchar_t* imageDirectory, int* selectedInfo) {
    // Thiết lập locale cho tất cả các tệp HTML để hỗ trợ UTF-8 sang UTF-16
    locale utf8_locale(locale(), new codecvt_utf8_utf16<wchar_t>);

    // Lặp qua từng sinh viên trong mảng students
    for (int i = 0; i < numStudents; ++i) {
        const Student& student = students[i];

        // Tạo đường dẫn cho tệp HTML
        wchar_t htmlFilePath[MAX_PATH];
        swprintf(htmlFilePath, MAX_PATH, L"%s\\%s.html", outputDirectory, student.id);

        // Mở tệp HTML để ghi
        wofstream htmlFile(htmlFilePath);

        // Kiểm tra xem tệp đã được mở thành công chưa
        if (htmlFile.is_open()) {
            // Thiết lập locale cho tệp HTML
            htmlFile.imbue(utf8_locale);

            // Ghi nội dung HTML
            htmlFile << L"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
            htmlFile << L"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\n";
            htmlFile << L"<head>\n";
            htmlFile << L"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
            htmlFile << L"<link rel=\"stylesheet\" type=\"text/css\" href=\"personal.css\" />\n";
            htmlFile << L"<title>HCMUS - " << student.name << L"</title>\n";
            htmlFile << L"</head>\n\n";
            htmlFile << L"<body>\n";
            htmlFile << L"<div class=\"Layout_container\">\n";
            htmlFile << L"<div class=\"Layout_MainContents\">\n";
            htmlFile << L"<div class=\"Personal_Main_Information\">\n";
            htmlFile << L"<div class=\"Personal_Location\">\n";
            htmlFile << L"<div class=\"Personal_FullName\">" << student.name << L" - " << student.id << L"</div>\n";
            htmlFile << L"<div class=\"Personal_Department\">" << student.department << L"</div>\n";
            htmlFile << L"<br />\n";
            htmlFile << L"<div class=\"Personal_Phone\">\n";
            htmlFile << L"Email: " << student.id << L"@gmail.com\n";
            htmlFile << L"</div>\n";
            htmlFile << L"<br />\n";
            htmlFile << L"<br />\n";
            htmlFile << L"</div>\n";
            htmlFile << L"<div class=\"Personal_HinhcanhanKhung\">\n";
            htmlFile << L"<img src=\"" << imageDirectory << L"\\" << student.image << L"\" class=\"Personal_Hinhcanhan\" />\n";
            htmlFile << L"</div>\n";
            htmlFile << L"</div>\n";
            htmlFile << L"<div class=\"MainContain\">\n";
            htmlFile << L"<div class=\"MainContain_Top\">\n";
            htmlFile << L"<div class=\"InfoGroup\">Thông tin cá nhân</div>\n";
            htmlFile << L"<div>\n";
            htmlFile << L"<ul class=\"TextInList\">\n";
            if (selectedInfo[0] == 1) {
                htmlFile << L"<li>Họ và tên: " << student.name << L"</li>\n";
            }
            if (selectedInfo[1] == 1) {
                htmlFile << L"<li>MSSV: " << student.id << L"</li>\n";
            }
            if (selectedInfo[2] == 1) {
                htmlFile << L"<li>Sinh viên khoa " << student.department << L"</li>\n";
            }
            if (selectedInfo[3] == 1) {
                htmlFile << L"<li>Ngày sinh: " << student.dob << L"</li>\n";
            }
            if (selectedInfo[4] == 1) {
                htmlFile << L"<li>Email: " << student.id << L"@gmail.com</li>\n";
            }
            htmlFile << L"</ul>\n";
            htmlFile << L"</div>\n";
            if (selectedInfo[5] == 1) {
                // Kiểm tra và ghi sở thích
                if (!student.hobby.empty()) {
                    htmlFile << L"<div class=\"InfoGroup\">Sở thích</div>\n";
                    htmlFile << L"<div>\n";
                    htmlFile << L"<ul class=\"TextInList\">\n";
                    for (const auto& hobby : student.hobby) {
                        htmlFile << L"<li>" << hobby << L"</li>\n";
                    }
                    htmlFile << L"</ul>\n";
                    htmlFile << L"</div>\n";
                }
            }

            // Ghi phần mô tả
            if (selectedInfo[6] == 1) {
                htmlFile << L"<div class=\"InfoGroup\">Mô tả</div>\n";
                htmlFile << L"<div class=\"Description\">\n";
                htmlFile << student.description << L"\n";
                htmlFile << L"</div>\n";
            }
            htmlFile << L"</div>\n";
            htmlFile << L"</div>\n";
            htmlFile << L"</div>\n";
            htmlFile << L"<div class=\"Layout_Footer\">\n";
            htmlFile << L"<div class=\"divCopyright\">\n";
            htmlFile << L"<br />\n";
            htmlFile << L"<br />\n";
            htmlFile << L"@2024</br>\n";
            htmlFile << L"Đồ án giữ kỳ</br>\n";
            htmlFile << L"Kỹ thuật lập trình</br>\n";
            htmlFile << L"TH2024</br>\n";
            htmlFile << L"22120319" << L" - " << L" Võ Tuấn Tài " << L"</br>\n";
            htmlFile << L"</div>\n";
            htmlFile << L"</div>\n";
            htmlFile << L"</div>\n";
            htmlFile << L"</body>\n";
            htmlFile << L"</html>\n";
            htmlFile.close(); // Đóng tệp HTML
            wcout << L" Thành công tạo các tệp " << student.id << L".html" << endl;
        }
        else {
            wcerr << L"Không thể tạo tệp HTML cho sinh viên " << student.id << endl;
        }
    }
}