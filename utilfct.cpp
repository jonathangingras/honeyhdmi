#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <QString>
#include <sys/types.h>
#include <sys/stat.h>

std::string exec(const char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

std::string get_escaped(const std::string str)
{
    std::ostringstream resStr;
    int len = str.length();
    int i;
    for(i = 0; i < len; i++)
    {
        if(str[i] == ' ' ||
           str[i] == '$' ||
           str[i] == '&' ||
           str[i] == '*' ||
           str[i] == '|' ||
           str[i] == '{' ||
           str[i] == '}' ||
           str[i] == '[' ||
           str[i] == ']' ||
           str[i] == '(' ||
           str[i] == ')' ||
           str[i] == '>' ||
           str[i] == '<' ||
           str[i] == '?' ||
           str[i] == '#'
          )

            resStr << '\\' << str[i];
        else
            resStr << str[i];
    }
    return resStr.str();
}

std::string get_filename(const std::string path)
{
    int len = path.length();
    int i;
    int lst_slsh_ptn = 0;
    std::ostringstream resStr;

    for(i = len; i > 0; i--)
    {
        if(path[i] == '/')
        {
            lst_slsh_ptn = i;
            break;
        }
    }

    for(i = lst_slsh_ptn + 1; i < len; i++)
    {
        resStr << path[i];
    }

    return resStr.str();
}

QString get_qstr_path(const QString remotePath, bool full_path, bool escaped)
{
    if(!full_path && !escaped)
        return QString::fromUtf8( get_filename(remotePath.toStdString() ).c_str() );
    else if(full_path && !escaped)
        return remotePath;
    else if(full_path && escaped)
        return QString::fromUtf8( get_escaped( remotePath.toStdString() ).c_str() );
    else if(!full_path && escaped)
        return QString::fromUtf8( get_escaped( get_filename( remotePath.toStdString() ) ).c_str() );
}

bool can_exe(std::string path)
{
    //FILE * pFile;

    struct stat sb;

    if (stat(path.c_str(), &sb) == 0 && sb.st_mode & S_IXUSR)
        return true;

    return false;
}
