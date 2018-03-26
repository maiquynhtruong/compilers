
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error! No input file...");
        exit(1);
    } else {
        init_scanner(argv[1]); 
    }
    while (next_token(&token) != T_END_OF_FILE) {
        print_token(&token); // process each token
//        reset_token(&token);
    }
    return 0;
}