#include "file.h"


/*
    List files in the current directory.
    `ls` is invoked without any arguments and lists the files in the current working directory.
*/
void ls(){
    DIR *dirc = NULL;
    char *cwd =(char*)malloc(sizeof(char)*1024);
   
    
    struct dirent *dpath = NULL;
    //struct dirent *oldpath = NULL;
    getcwd(cwd,1024);
    dirc = opendir(cwd);
    int cnt = 0;
    while((dpath = readdir(dirc)) != NULL){
       if(strcmp(dpath->d_name, ".")!=0 && (strcmp(dpath->d_name, "..")!= 0)){
            if(cnt>=2) printf("%s  ",dpath->d_name);
            
        }
        cnt++;
    }
    printf("\n");
    free(cwd);
    closedir(dirc);

}

/*
    Create file 
    Be careful when you set permission about your file.
*/
void create_file(char* filename) {
    //printf("%s\n",filename);
    int fdt = open(filename,O_RDONLY,0777);
    if(fdt<0){
        int fd = creat(filename,0777);
        //nt fd = open(filename, O_WRONLY | O_CREAT| O_TRUNC, 0777);
        if(fd<0){   
            printf("file create failed");
        }
        close(fd);
    }
    close(fdt);
}

/*
    Read 'size' bytes in file and print it.
    If there is no such file, print ("error occured\n").
*/
void read_file(char* filename, int size) {
    int fd;
    fd = open(filename, O_RDONLY, 0777);
    if(fd<0){
        close(fd);
        printf("error occured\n");
    }
    else{
        char *content = (char *)malloc(sizeof(char)*(size+1));
        int blen = read(fd, content, size);
        if(blen <=0 ){
            printf("\n");
        }
        else{
            int c_len = strlen(content);
        content[blen] = '\0';
        printf("%d ",size);
        puts(content);
        close(fd);
        free(content);
            
        
        }
        
        }
}

/*
	Open file and write char array tmp to your file.
*/
void write_file(char* filename, char* tmp) {
    int fd;
    fd = open(filename, O_WRONLY | O_CREAT |O_TRUNC, 0777);
    if(fd<0){
        printf("file open failed");
    }
    else{
        write(fd, tmp, strlen(tmp));
        close(fd);
    }
}

/* 
    Copy the contents of source file to the dest file back and forth as much as n bytes.For example, suppose "1234abcd\n" is the content of the source file. Then "1a2b3c4d\n" will be the content of the dest file. You sholud use lseek() function.
    If there is no dest file, create it by using "create_file" funtion
*/
void mixup_copy_file(char* source, char* dest,int n) {
    int fd1; //source
    fd1 = open(source, O_RDONLY,0777);
    int fd2; //dest
    fd2 = open(dest, O_RDWR |O_CREAT|O_TRUNC, 0777);
    
    for(int i=0; i<n; i++){
        char dup_buf[10]; // read char from source
        lseek(fd1, i, SEEK_SET); //forward
        read(fd1, dup_buf, 1);
        write(fd2, dup_buf, 1);
        
        lseek(fd1, n+i, SEEK_SET); //back
        read(fd1, dup_buf, 1);
        write(fd2, dup_buf, 1);
    }
    char chk_buf[10];
    lseek(fd1, -1, SEEK_END);
    read(fd1, chk_buf,1);
    if(chk_buf[0] == '\n'){
        write(fd2, "\n", 1);
    }
    close(fd1);
    close(fd2);
}

/*
    Get 'command' as an input,
    and execute proper functionalities according to the command line.
    Only `create`, `write`, `read`, `copy`, `ls` commands are allowed.
    If `file` takes input from one of those commands, it should return "True", otherwise "False".
    
*/
int file(char* command) {
    char cpycommand[1024];
    strcpy(cpycommand, command);
    int ptr=0; //strating point
    if(cpycommand[ptr] == ' '){
        while(cpycommand[ptr] != ' '){
            ptr++;
        }
        ptr++;
    }
    
    if(strncmp(cpycommand+ptr, "ls", 2)==0){
        ls();
        return True;
    }
    else if(strncmp(cpycommand+ptr, "create",6)==0){
        int ptr_f= ptr + 6 ;//where filename starts
        while(cpycommand[ptr_f] == ' '){
            ptr_f++;
        }
        
        
        int ptr_fe = ptr_f;//file name end
        while(cpycommand[ptr_fe] != ' ' && cpycommand[ptr_fe] != '\n'){
            ptr_fe++;
        }
        ptr_fe--;
        //printf("it's here\n");
        char filename[100] = {'\0',};
        int j=0;
        for(int i=ptr_f ; i<=ptr_fe;i++){
            filename[j] = cpycommand[i];
            j++;
        }
        //printf("%s",filename);
        //printf("it's here22\n");
        create_file(filename);
        
        return True;
        
    }
    
    else if(strncmp(cpycommand+ptr, "write",5)==0){
        int ptr_f = ptr+ 5 ; //where file starts
        while(cpycommand[ptr_f] == ' '){
            ptr_f++;
        }
        
        int ptr_fe = ptr_f;//file name end
        while(cpycommand[ptr_fe] != ' '){
            ptr_fe++;
        }
        ptr_fe--;
        
        int ptr_ts = ptr_fe+1; //tmp start
        while(cpycommand[ptr_ts] == ' '){
            ptr_ts++;
        }
        
        int ptr_te = ptr_ts; //tmp end right before \n
        while(cpycommand[ptr_te] != ' ' && cpycommand[ptr_te] != '\0'){
            ptr_te++;
        }
        ptr_te--;
        
        char filename[100] = {'\0',};
        char temp[100] = {'\0', };
        int j=0;
        for(int i=ptr_f ; i<=ptr_fe;i++){
            filename[j] = cpycommand[i];
            j++;
        }
        j=0;
        for(int i=ptr_ts; i<=ptr_te;i++){
            temp[j] = cpycommand[i];
            j++;
        }
        
        write_file(filename, temp);
        return True;
    }
    
    else if(strncmp(cpycommand+ptr, "read",4)==0){
        int ptr_f = ptr+ 5 ; //where file starts
        while(cpycommand[ptr_f] == ' '){
            ptr_f++;
        }
        
        int ptr_fe = ptr_f;//file name end
        while(cpycommand[ptr_fe] != ' '){
            ptr_fe++;
        }
        ptr_fe--;
        
        int ptr_ts = ptr_fe+1; //tmp start
        while(cpycommand[ptr_ts] == ' '){
            ptr_ts++;
        }
        
        int num = atoi(cpycommand+ptr_ts);
        
        char filename[100] = {'\0',};
        int j=0;
        for(int i=ptr_f ; i<=ptr_fe;i++){
            filename[j] = cpycommand[i];
            j++;
        }
        
        read_file(filename, num);
        return True;
    }
    
    else if(strncmp(cpycommand+ptr, "copy",4)==0){
        int ptr_f = ptr+ 4 ; //where file starts
        while(cpycommand[ptr_f] == ' '){
            ptr_f++;
        }
        
        int ptr_fe = ptr_f;//file name end
        while(cpycommand[ptr_fe] != ' '){
            ptr_fe++;
        }
        ptr_fe--;
        
        int ptr_ts = ptr_fe+1; //tmp start
        while(cpycommand[ptr_ts] == ' '){
            ptr_ts++;
        }
        
        int ptr_te = ptr_ts; //tmp end 
        while(cpycommand[ptr_te] != ' ' ){
            ptr_te++;
        }
        ptr_te--;
        
        int ptr_iter = ptr_te+1;
        while(cpycommand[ptr_iter] == ' '){
            ptr_iter++;
        }
        
        int iter = atoi(cpycommand+ptr_iter);
        char filename[100] = {'\0',};
        char temp[100] = {'\0', };
        int j=0;
        for(int i=ptr_f ; i<=ptr_fe;i++){
            filename[j] = cpycommand[i];
            j++;
        }
        j=0;
        for(int i=ptr_ts; i<=ptr_te;i++){
            temp[j] = cpycommand[i];
            j++;
        }
        
        mixup_copy_file(filename, temp, iter);
        return True;
    }
    
    
    return False;
}
