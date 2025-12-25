#include "utils.h"

// 定义一个函数，返回当前的系统时间转换为std::string对象
std::string get_current_time_string() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto local_time = std::localtime(&time);
    char buffer[20];
    // 使用strftime函数，将tm结构体转换为指定格式的字符串，存储在buffer中
    std::strftime(buffer, sizeof(buffer), "%Y_%m_%d_%H_%M_%S", local_time);
    // 将char数组转换为std::string对象，返回该对象
    return std::string(buffer);
}

// 定义一个函数，接受两个文件夹路径作为参数，将源文件夹中的所有文件打包成一个buf格式的文件，并备份到目标文件夹中
void backup_folder_as_buf(const fs::path& source, const fs::path& destination, 
    std::map<std::string, std::string> condition) {
    if (fs::exists(source) && (fs::is_directory(source) || fs::is_regular_file(source))) {
        if (fs::exists(destination) && fs::is_directory(destination)) {
            // 创建一个buf类的对象，传入源文件夹的路径作为参数，递归遍历该文件夹下所有文件，并将它们转换为File结构体并存储到buf对象中
            buf r(source, source, condition);
            r.buildHuffCoder();
            // 定义一个buf格式的文件路径，以源文件夹的名称作为buf文件的名称，并添加".buf"作为扩展名
            fs::path buf_file = destination / (source.filename().string() + "_" + get_current_time_string() + ".buf");
            // 调用buf对象的write_buf函数，传入buf格式的文件路径作为参数，将buf对象中的files向量中的所有File结构体写入到该buf格式的文件中
            unsigned short crc_code = r.write_buf(buf_file);
            std::string ab = fs::absolute(buf_file).string();
            ab = ab.substr(0, ab.size() - 4) + "_" + std::to_string(crc_code) + ".buf";
            fs::path crc_buf_file = ab;
            fs::rename(buf_file, crc_buf_file);

            std::cout << "Backup: " << source << " to " << crc_buf_file << std::endl;
        }
        else {
            std::cerr << "Error: 不合法的目标路径 " << destination << std::endl;
            throw std::invalid_argument("invalid destination folder path");
        }
    }
    else {
        std::cerr << "Error: 不合法的源路径 " << source << std::endl;
        throw std::invalid_argument("invalid source folder path");
    }
}

// 定义一个函数，将一个磁盘中的buf文件读到内存中，is_decrypt 表示是否经过解密得到
buf load_buf(const fs::path& buf_file, std::string crc, bool is_decrypt) {
    buf loaded_file;
    std::ifstream in(buf_file, std::ios::binary);
    if (in.is_open()) {
        // 先读HuffmanCoder
        // 读取encoder的大小
        size_t encoder_size;
        in.read(reinterpret_cast<char*>(&encoder_size), sizeof(encoder_size));
        for (size_t i = 0; i < encoder_size; i++) {
            char key;
            in.read(reinterpret_cast<char*>(&key), sizeof(key));
            // 输入错误的密码会导致文件不符合设定，因此要时刻判断
            if (is_decrypt && !in.good()) {
                std::cerr << "Error: 密码错误，请重试！" << std::endl;
                throw std::runtime_error("Wrong password");
            }
            size_t value_size;
            in.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));
            if (is_decrypt && !in.good()) {
                std::cerr << "Error: 密码错误，请重试！" << std::endl;
                throw std::runtime_error("Wrong password");
            }
            std::string value(value_size, '\0');
            in.read(value.data(), value_size);
            if (is_decrypt && !in.good()) {
                std::cerr << "Error: 密码错误，请重试！" << std::endl;
                throw std::runtime_error("Wrong password");
            }
            loaded_file.huff_coder.encoder.insert({ key, value });
        }
        // 读取decoder的大小
        size_t decoder_size;
        in.read(reinterpret_cast<char*>(&decoder_size), sizeof(decoder_size));
        for (size_t i = 0; i < decoder_size; i++) {
            size_t key_size;
            in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
            if (is_decrypt && !in.good()) {
                std::cerr << "Error: 密码错误，请重试！" << std::endl;
                throw std::runtime_error("Wrong password");
            }
            std::string key(key_size, '\0');
            in.read(key.data(), key_size);
            if (is_decrypt && !in.good()) {
                std::cerr << "Error: 密码错误，请重试！" << std::endl;
                throw std::runtime_error("Wrong password");
            }
            char value;
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
            if (is_decrypt && !in.good()) {
                std::cerr << "Error: 密码错误，请重试！" << std::endl;
                throw std::runtime_error("Wrong password");
            }
            loaded_file.huff_coder.decoder.insert({ key, value });
        }

        std::string result = "";
        // 计算除掉HuffCoder后文件的大小。注意还要减去两个size_t的大小
        // 获取当前的读取位置
        std::streampos cur_pos = in.tellg();
        if (is_decrypt && !in.good()) {
            std::cerr << "Error: 密码错误，请重试！" << std::endl;
            throw std::runtime_error("Wrong password");
        }
        // 移动到文件的末尾
        in.seekg(0, std::ios::end);
        // 获取文件的总大小
        std::streampos total_size = in.tellg();
        if (is_decrypt && !in.good()) {
            std::cerr << "Error: 密码错误，请重试！" << std::endl;
            throw std::runtime_error("Wrong password");
        }
        // 计算剩下未读部分的大小
        std::streampos remain_size = total_size - cur_pos;
        // 恢复原来的读取位置
        in.seekg(cur_pos);
        size_t files_size = remain_size;
        char* buffer = new char[files_size];
        in.read(buffer, files_size);
        if (is_decrypt && !in.good()) {
            std::cerr << "Error: 密码错误，请重试！" << std::endl;
            throw std::runtime_error("Wrong password");
        }
        in.close();

        std::string crc_code = std::to_string(crc16(buffer, files_size));
        if (crc == crc_code) {
            std::cout << "CRC 校验正确！将解析 buf 文件..." << std::endl;
        }
        else {
            std::cout << "CRC 校验错误！是否继续解析？（按 y 继续）" << std::endl;
            char op;
            std::cin >> op;
            if (op != 'y') {
                exit(0);
            }
        }

        // 遍历buffer中的每个字节
        for (size_t i = 0; i < files_size; i++) {
            // 把每个字节转换成8位的二进制字符串
            std::bitset<BITS_PER_BYTE> bits(buffer[i]);
            // 把二进制字符串追加到result中
            result += bits.to_string();
        }
        delete[] buffer;

        std::string decoded_files_string = loaded_file.huff_coder.decode(result);
        
        loaded_file.parse_buf_str(decoded_files_string);
        return loaded_file;
    } else {
        std::cerr << "Error: buf 文件打开失败 " << buf_file << std::endl;
        throw std::runtime_error("cannot open buf file");
    }
}

// 定义一个函数，接受一个文件夹路径和一个文件名作为参数，删除该文件夹中的指定文件
void delete_file_in_folder(const std::filesystem::path& folder_path, const std::string& file_name) {
    if (std::filesystem::exists(folder_path) && std::filesystem::is_directory(folder_path)) {
        for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
            // 检查条目是否是一个文件，并且文件名是否与参数匹配
            if (std::filesystem::is_regular_file(entry) && entry.path().filename() == file_name) {
                std::filesystem::remove(entry);
                break;
            }
        }
    }
    else {
        throw std::runtime_error("Invalid folder path: " + folder_path.string());
    }
}

// 定义一个函数，接受两个文件夹路径作为参数，将一个buf格式的文件还原为若干个原文件，并保存到目标文件夹中
void restore_folder_from_buf(const fs::path& buf_file, const fs::path& destination, bool is_decrypt) {
    if (fs::exists(buf_file) && fs::is_regular_file(buf_file)) {
        if (fs::exists(destination) && fs::is_directory(destination)) {
            // 先得到记录的CRC
            std::string buf_name = buf_file.filename().string();
            size_t lastUnderscorePos = buf_name.rfind("_");
            size_t dotPos = buf_name.rfind(".");
            std::string result = buf_name.substr(lastUnderscorePos + 1, dotPos - lastUnderscorePos - 1);
            std::cout << "crc: " << result << "正在校验中" << std::endl;
            // 创建一个buf类的对象，传入buf格式的文件路径作为参数，将该buf格式的文件解析为多个File结构体，并存储到buf对象中
            buf r = load_buf(buf_file, result, is_decrypt);
            // 遍历buf对象中的files向量中的所有File结构体
            for (const auto& file : r.files) {
                fs::path original_file = destination / file.name;
                // 调用write_file函数，传入File结构体和原始格式的文件路径作为参数，将File结构体写入到指定的原始格式的文件中
                write_file(file, original_file);
                std::cout << "Restored: " << original_file << " from " << buf_file << std::endl;
            }
        }
        else {
            std::cerr << "Error: 不合法的目标路径 " << destination << std::endl;
            throw std::invalid_argument("invalid destination folder path");
        }
    } else {
        std::cerr << "Error: 不合法的 buf 文件 " << buf_file << std::endl;
        throw std::invalid_argument("invalid buf file path");
    }
}

// 定义一个函数，对磁盘上buf文件加密，产生加密后的文件ebuf(encrypted_buf)
void encrypt_buf(const fs::path& buf_file, const char* key) {
    std::ifstream in(buf_file, std::ios::binary);
    size_t file_size = fs::file_size(buf_file);

    std::string bin_data(file_size, '\0');

    // 读取文件的内容，一次性读取整个文件
    in.read(bin_data.data(), file_size);
    in.close();
    size_t len = 0;
    char* encrypt_data = (char*)xxtea_encrypt(bin_data.c_str(), bin_data.size(), key, &len);
    

    // 将加密后文件写入磁盘，路径为buf所在文件夹，后缀改成.ebuf
    fs::path ebuf_file = buf_file.string().substr(0, buf_file.string().size() - 4) + ".ebuf";
    std::ofstream out(ebuf_file, std::ios::binary);
    out.write(encrypt_data, len);
    out.close();

    // 删除原buf文件（不然还加什么密）
    fs::remove(buf_file);
}

// 定义一个函数，将磁盘上ebuf文件解密为buf文件，还原到指定位置
void decrypt_ebuf(const fs::path& ebuf_file, const char* key, const fs::path& destination) {
    // 打开文件，以二进制模式读取
    std::ifstream in(ebuf_file, std::ios::binary);
    size_t file_size = fs::file_size(ebuf_file);

    std::string bin_data(file_size, '\0');

    in.read(bin_data.data(), file_size);
    in.close();

    size_t len = 0;

    // std::cout<<"start decrypt"<<std::endl;
    char* decrypt_data = (char*)xxtea_decrypt(bin_data.c_str(), bin_data.size(), key, &len);
    // std::cout<<"finish decrypt"<<std::endl;

    // 将解密后文件写入磁盘
    fs::path buf_file = ebuf_file.string().substr(0, ebuf_file.string().size() - 5) + ".buf";
    std::ofstream out(buf_file, std::ios::binary);
    out.write(decrypt_data, len);
    out.close();

    restore_folder_from_buf(buf_file, destination, true);
}


std::string calc_buf_crc(const fs::path& buf_file) {
    // 读取文件，计算CRC校验码
    std::ifstream in(buf_file, std::ios::binary);
    size_t file_size = fs::file_size(buf_file);
    std::string bin_data(file_size, '\0');
    // 读取文件的内容，一次性读取整个文件
    in.read(bin_data.data(), file_size);
    in.close();

    unsigned short crc_code = crc16((char *)bin_data.c_str(), bin_data.size());
    return std::to_string(crc_code);
}