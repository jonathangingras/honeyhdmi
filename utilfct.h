#ifndef UTILFCT_H
#define UTILFCT_H

#include <string>
#include <QString>

std::string exec(const char* cmd);

std::string get_escaped(const std::string str);

std::string get_filename(const std::string path);

QString get_qstr_path(const QString remotePath, bool full_path, bool escaped);

#endif // UTILFCT_H
