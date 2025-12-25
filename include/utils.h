#ifndef utils_h
#define utils_h

#include "File.h"
#include "HuffmanCoder.h"
#include "buf.h"
#include "xxtea.h"
#include "crc.h"

// ����һ�����������ص�ǰ��ϵͳʱ��ת��Ϊstd::string����
std::string get_current_time_string();

// ����һ�����������������ļ���·����Ϊ��������Դ�ļ����е������ļ������һ��buf��ʽ���ļ�
//�������ݵ�Ŀ���ļ�����
void backup_folder_as_buf(const fs::path& source, const fs::path& destination,
	std::map<std::string, std::string> condition = std::map<std::string, std::string>());

// ����һ����������һ�������е�buf�ļ������ڴ��У�ͬʱ���CRC��
buf load_buf(const fs::path& buf_file, std::string crc, bool is_decrypt);

// ����һ������������һ���ļ���·����һ���ļ�����Ϊ������ɾ�����ļ����е�ָ���ļ�
void delete_file_in_folder(const std::filesystem::path& folder_path, const std::string& file_name);

// ����һ�����������������ļ���·����Ϊ��������һ��buf��ʽ���ļ���ԭΪ���ɸ�ԭ�ļ��������浽Ŀ���ļ�����
void restore_folder_from_buf(const fs::path& buf_file, const fs::path& destination, bool is_decrypt);

// ����һ���������Դ�����buf�ļ����ܣ��������ܺ���ļ�ebuf(encrypted_buf)
void encrypt_buf(const fs::path& buf_file, const char* key);

// ����һ���������Դ�����ebuf�ļ����ܣ���ԭ��ָ��λ��
void decrypt_ebuf(const fs::path& ebuf_file, const char* key, const fs::path& destination);

// ����һ���������Դ�����buf�ļ�����CRC
// std::string calc_buf_crc(const fs::path& buf_file);
#endif