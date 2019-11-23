
		if(strcmp(lexeme,"k") == 0){
							char c;
							printf("\n%s\n",lexeme);
							while(fread(&c,1,1,source))	printf("%c", c);
		}
