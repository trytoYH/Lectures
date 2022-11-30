#include "ftp_test.h"

// NOTE: Please make sure that you fully understand the description before starting the coding. It will save your time!

// Initialize a client socket.
// To ensure the client socket connect with the target server,
// you should pay attention to set the parameters "ip address" and "port" correctly.
// Remind that we assumed "./client -h 127.0.0.1 -p 8080".
int init(int argc, char *argv[]){
   // printf("starts init\n");
  /*####################################################################################*/
  /*###################### DO NOT MODIFY THE CODE BELOW THIS LINE ######################*/
  int sock;
  if(argc != 5 || strcmp(argv[1], "-h") || strcmp(argv[3], "-p")){
    printf("unvalid command line\n");
    printf("./client -h <address> -p <port>\n");
    exit(1);
  }
  /*###################### DO NOT MODIFY THE CODE ABOVE THIS LINE ######################*/
  /*####################################################################################*/

  // client socket initialization
  /* Your Code */
  sock = socket( PF_INET, SOCK_STREAM, 0);
  if(sock<0){
    return False;
  }
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  // connection test
  /* Your Code */
  if((connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))<0){
    return False;
  }
  return sock; // return initialized socket
}

// Implement "ls" function.
// NOTE: This is optional and will not be used in grading. 
// For those who get stuck in error, this function may help you get a current directory or something related to this assignment.
void ls(){
// copy from my asgginment2
    DIR *dirc = NULL;
    char *cwd =(char*)malloc(sizeof(char)*1024);
   
    
    struct dirent *dpath = NULL;
    getcwd(cwd,1024);
    dirc = opendir(cwd);
    int cnt = 0;
    while((dpath = readdir(dirc)) != NULL){
       if(strcmp(dpath->d_name, ".")!=0 && (strcmp(dpath->d_name, "..")!=  0)){
            if(cnt>=2) printf("%s  ",dpath->d_name);
            
        }
        cnt++;
    }
    printf("\n");
    free(cwd);
    closedir(dirc);
}

// Implement "changedir" function.
// Change the current directory to target directory.
void changedir(const char* name){ 
  int change = chdir(name);
  if(change != 0){
    //printf("changedir failed");
  }
}

// Implement "Upload" function.
// Input : socket & file_name you want to upload to server
// Return : if the socket success to upload the accesible file, return "True". Otherwise, return "False".
int upload(int sock, const char* file_name){
    
    changedir("client_files");
    FILE* tarfile = fopen(file_name, "rb");
    if(tarfile ==NULL){
        printf("file not open\n");
        return False;
    }
    char mes[20] = "upload";
    send(sock,mes,20, 0 );
    char buf[32000]={};
    fread(buf, sizeof(buf),1, tarfile);
    fclose(tarfile);
    send ( sock, file_name, 20, 0);
    int removechk = remove(file_name);
    int upchk = send(sock, buf,sizeof(buf), 0);
    
    if(upchk == -1){
        return False;
    }
    else{
        return True;
    }
    
    
  
  
}

// Implement "Download" function.
// Input : socket & file_name you want to download from server
// Return : if the socket success to download the accessible file, return "True". Otherwise, return "False".
int download(int sock, const char* file_name){
    char mes[20] = "download";
    send(sock,mes,20, 0 );
    int sigchk = send(sock, file_name, 20, 0);
    
    char buf[32000]={};
    int recchk = recv(sock, buf, sizeof(buf), 0);
    changedir("client_files");
    FILE * dfile = fopen(file_name, "wb");
    if(dfile == NULL){
        return False;
    }
    fwrite(buf,sizeof(buf),1, dfile);
    fclose(dfile);
    
    
  if(recchk == -1){
    return False;
  }
  else{
    return True;
  }
}

// Implement "Terminate" function.
// Terminate function should send the terminate signal to server.
void terminate(int sock){
    char mes[20] = "terminate";
    send(sock,mes,sizeof(mes), 0 );
    printf("terminate done\n");
    close(sock);
    
    
    
}
