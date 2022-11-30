#include "ftp_test.h"

// NOTE: Please make sure that you fully understand the description before starting the coding. It will save your time!

int main(int argc, char *argv[]){

  /*###################################################################################*/
  /*###################### DO NOT MODIFY THE CODE BELOW THIS LINE ######################*/
  int serv_sock; // server socket
  int clnt_sock; // client socket
  if(argc != 3 || strcmp(argv[1], "-p")){
    printf("invalid command line\n");
    printf("./server -p <port>\n");
    exit(1);
  }
  /*###################### DO NOT MODIFY THE CODE ABOVE THIS LINE ######################*/
  /*####################################################################################*/
  
  // Initialize a server socket.
  // To ensure the server socket connect with client socket, 
  // you should initialize the server socket with the proper settings.
  // Note that you should use "INADDR_ANY" option when you setup the server address.
  
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  
  if(serv_sock ==-1){
    printf("socket error\n");
    return False;
  }
  
  // Bind the server socket .
  int bt = bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if(bt == -1){
    printf("bind error\n");
    return False;
  }

  // Set the server socket to listening mode.
  
  int lt = listen( serv_sock, 10) ;
  printf("Server start: listen on port %s\n", argv[2]);
  if(lt != 0){
    printf("listen error\n");
    return False;
  }

  // Accept client sockets if they request to accept.
  
  unsigned int clnt_size;
  clnt_size = sizeof(clnt_addr);
  clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_size);
  if(clnt_sock == -1){
    printf("accept error\n");
    return False;
  }
  
  
  // By now, we have initialized the server socket and made the socket accept the client sockets.
  // We will then implement the auxiliary loop to let the server socket dealing with the requests from the clients.
  // Funtions that you should implement are as follows: "Upload", "Download", and "Terminate".
  while(True){
    char mbuf[20];
    char buf[32000] = {};
    char name[50];
    int smes = recv(clnt_sock, mbuf, sizeof(mbuf), 0);
    // upload
    if(strcmp(mbuf, "upload")==0){
        changedir("server_files");
        recv(clnt_sock, name, 50, 0 );
        
        FILE * sfile = fopen(name, "wb");
        
        if(sfile == NULL){
            printf("file open failed\n");
            fclose(sfile);
            return False;
         
        }
        
        int chk = recv(clnt_sock, buf, sizeof(buf), 0);
        if(chk == -1){
            fclose(sfile);
            return False;
           
        }
        
        fwrite(buf,sizeof(buf),2,sfile);
        
        fclose(sfile);
        
    }

    // download
    else if(strcmp(mbuf, "download")==0){
        
        changedir("server_files");
        recv(clnt_sock, name, 50, 0);
        FILE * sfile = fopen(name, "rb");
        
        if(sfile == NULL) {
            return False;
        }
        
        fread(buf, sizeof(buf),1, sfile);
        fclose(sfile);
        int removechk = remove(name);
        
        
        send(clnt_sock, buf, sizeof(buf), 0);
        
        
      
    }

    // Terminate
    else if(strcmp(mbuf, "terminate")==0){
        close(clnt_sock);
        close(serv_sock);
       break;
      
    }
  }
}
