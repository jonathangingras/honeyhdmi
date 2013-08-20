#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUF_LEN 128

char * exec(const char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[BUF_LEN];
    
    fseek(pipe, 0L, SEEK_END);
    int table_len = ftell(pipe);
    fseek(pipe, 0L, SEEK_SET);

    char * res = (char *) malloc( (size_t)( table_len + sizeof(char) ) );

    fgets(res, BUF_LEN, pipe);

    while(!feof(pipe))
    {
        if(fgets(buffer, BUF_LEN, pipe) != NULL)
            strcat(res, buffer);
    }
    pclose(pipe);

    return res;
}

int main(int argc, char const *argv[])
{
	char * openbox = "openbox";
	char ** session;

	if(argc == 2)
	{
		session = &argv[1];
	}
	else if(argc == 1)
	{
		session = &openbox;
	}
	else
		return 1;

	const char * user = getlogin();
	const char * path = getenv("PATH");
	const char * s_mng = getenv("SESSION_MANAGER");

	const char * user_decl_ = "USER=";
	const char * home_decl_ = "HOME=/home/";
	const char * path_decl_ = "PATH=";
	const char * s_mng_decl_ = "SESSION_MANAGER=";

	char * user_dest = (char *) malloc( sizeof(char) * (strlen(user) + strlen(user_decl_) + 1 ) );
	char * home_dest = (char *) malloc( sizeof(char) * (strlen(user) + strlen(home_decl_) + 1 ) );
	char * path_dest = (char *) malloc( sizeof(char) * (strlen(path) + strlen(path_decl_) + 1 ) );
	char * s_mng_dest = (char *) malloc( sizeof(char) * (strlen(s_mng) + strlen(s_mng_decl_) + 1 ) );

	strcpy(user_dest, user_decl_);
	strcpy(home_dest, home_decl_);
	strcpy(path_dest, path_decl_);
	strcpy(s_mng_dest, s_mng_decl_);

	strcat(user_dest, user);
	strcat(home_dest, user);
	strcat(path_dest, path);
	strcat(s_mng_dest, s_mng);

	printf("%s\n", *session);

	char *env[] = {user_dest, home_dest, path_dest, s_mng_dest, NULL};

	char *args[] = {"hdmi", *session, NULL};

	execvpe("hdmi", args, env);
}