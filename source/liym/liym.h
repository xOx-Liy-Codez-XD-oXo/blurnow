#ifndef LIYM_H
#define LIYM_H

#ifdef GUY_DEV
#define LIYM_ALLOW_ERROR_PRINTING
#define LIYM_ALLOW_INFO_PRINTING
//#define LIYA_ALLOW_STREAM_INFO_PRINTING
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "liymutils.h"

struct liymParseResults {
	int filetype;
	int tricount;
	float *vertpos;
	float *vertnor;
	float *facenor;
	float *texcord;
	int   *vcindex;
	int   *vertcol;
	int   *mtxidx;
};

struct liymParseResults openLiymFromFile(int mdlindex, char *infilename, FILE *liymfile) {
	#ifdef LIYM_ALLOW_ERROR_PRINTING
		printf("Liym parser was compiled with error printing enabled\n");
	#endif
	#ifdef LIYM_ALLOW_INFO_PRINTING
		printf("Liym parser was compiled with info printing enabled\n");
	#endif

	struct liymParseResults result;
	result.filetype = 255; //filetypes >=16 currently impossible
	char cur;

	if ((liymfile = fopen(infilename, "r")) == NULL) {
		#ifdef LIYM_ALLOW_ERROR_PRINTING
			printf("### Couldn't open file\n");
		#endif
		result.filetype = 0;
		return result;
	}

	fseek(liymfile, 0, SEEK_SET);
	
	// Find where the requested object is in the file

		// Loop over file once to find how many objects there are

		int objectcount = 0; 
		while ((cur = fgetc(liymfile)) != EOF) {
			//printf("'%d' ", cur);
			if (cur == 'o') {
				objectcount++;
			}
			if(cur == 255 || cur == EOF) { break; }
		}

		if(objectcount < mdlindex + 1) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### Model index out of range\n");
			#endif
			result.filetype = 0;
			return result;
		}

		int *objectlocations;
		objectlocations = malloc(sizeof(int) * objectcount);	

		// Loop over the file a second time to find the location of every object
		fseek(liymfile, 0, SEEK_SET);

		objectcount = 0; //using this var for how many o's we've found now
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 'o') {
				objectlocations[objectcount] = ftello(liymfile) - 1;
				objectcount++;
			}
			if(cur == 255 || cur == EOF) { break; }
		}

		int requestedobjectlocation = objectlocations[mdlindex];

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("requested object is at %d\n", requestedobjectlocation);
		#endif

		free(objectlocations);



	// Figure out the filetype and file features
	
		fseek(liymfile, requestedobjectlocation + 1, SEEK_SET); //Seek past the o

		int vertpospresent = 0; // 0000 0001
		int facenorpresent = 0; // 0000 0010
		int vertnorpresent = 0; // 0000 0100
		int texcordpresent = 0; // 0000 1000
		int vcindexpresent = 0; // 0001 0000
		int vertcolpresent = 0; // 0010 0000
		int mtxidxpresent = 0;  // 0100 0000
		int extendpresent = 0;  // 1000 0000

		int unsupported = 0;
		
		if(vertpospresent || facenorpresent || vertnorpresent || texcordpresent || vcindexpresent || vertcolpresent || mtxidxpresent || extendpresent || unsupported) {} //silence a warning

		int intfiletype;
		int8_t bytefiletype;

		{
			char filetypestr[5] = "aaaaa";

			while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
			filetypestr[0] = cur;
			int i = 1;		
			while (liym_isAlphaNumeric(cur = fgetc(liymfile))) {
				filetypestr[liym_min(i, 4)] = cur; //memory safety for slackers
				i++;
			}

			intfiletype = atoi(filetypestr);
			bytefiletype = (int8_t)intfiletype;
		
			#ifdef LIYM_ALLOW_INFO_PRINTING
				printf("Filetype is %d\n", intfiletype);
			#endif
		}//dont let temp variables pollute later bits

		if(bytefiletype & 0b00000001) { vertpospresent = 1; } //Look for p table
		if(bytefiletype & 0b00000010) { facenorpresent = 1; } //Look for n table and put in facenor
		if(bytefiletype & 0b00000100) { vertnorpresent = 1; } //Look for n table
		if(bytefiletype & 0b00001000) { texcordpresent = 1; } //Look for t table
		if(bytefiletype & 0b00010000) { vcindexpresent = 1; } //Look for c table
		if(bytefiletype & 0b00100000) { vertcolpresent = 1; } //Look for c table
		if(bytefiletype & 0b01000000) { mtxidxpresent = 1; }  //Look for w table
		if(bytefiletype & 0b10000000) { unsupported = 1; }

		if(unsupported == 1) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### File contains unsupported features\n");
			#endif
			result.filetype = 0;
			return result;
		}

		result.filetype = intfiletype;

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("File features:	\n");
			printf("	Vertex positions:     %s\n", (vertpospresent == 1) ? "./" : "X");
			printf("	Face normals:         %s\n", (facenorpresent == 1) ? "./" : "X");
			printf("	Per-vertex nors:      %s\n", (vertnorpresent == 1) ? "./" : "X");
			printf("	Texture coords:       %s\n", (texcordpresent == 1) ? "./" : "X");
			printf("	Indexed vert colors:  %s\n", (vcindexpresent == 1) ? "./" : "X");
			printf("	Direct vertex colors: %s\n", (vertcolpresent == 1) ? "./" : "X");
			printf("	Matrix indecies:      %s\n", (mtxidxpresent == 1 ) ? "./" : "X");
		#endif

	// Find triangle count

		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		unsigned int tricount = 0;

		{
			char tricountstr[10] = "aaaaaaaaaa";
			tricountstr[0] = cur;
			int i = 1;
			while (liym_isAlphaNumeric(cur = fgetc(liymfile))) {
				tricountstr[liym_min(i, 9)] = cur;
				i++;
			}

			tricount = atoi(tricountstr);
		}
			
		result.tricount = tricount;

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("Tri count is %d\n", tricount);
		#endif
	
	// Search for a p block 

	if (vertpospresent == 1) {
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 'p') {
				break;
			}
		}
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		int seekback = ftello(liymfile) - 1;
		fseek(liymfile, seekback, SEEK_SET);

		int pblocksize = 0;
		int pmaxfloatsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
				}
				pmaxfloatsize = liym_max(i, pmaxfloatsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
				
				pblocksize++;
			}
		}
		
		if(pblocksize / 9 != tricount) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### P block size inconsistent with triangle count.\n");
			#endif
			result.filetype = 0;
			return result;
		}

	// Read values from p block

		result.vertpos = malloc(pblocksize * sizeof(float));
	
		fseek(liymfile, seekback - 1, SEEK_SET);
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
		fseek(liymfile, -1L, SEEK_CUR);
		//Should be on the carriage return now


		char *tempfloat;
		tempfloat = malloc(pmaxfloatsize * sizeof(char));
		for(int b = 0; b < pmaxfloatsize; b++) {
			tempfloat[b] = 'a';
		}	

		{		
			int j = 0;
			while (liym_isAny(cur)) {
				for(int b = 0; b < pmaxfloatsize; b++) {
					tempfloat[b] = 'a';
				}
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					tempfloat[i] = cur;
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					//printf("%s\n", tempfloat);
					result.vertpos[j] = atof(tempfloat);	
				} else {
					break;
				}
				j++;
			}
		}
		free(tempfloat);

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("p block read. sample:\n     %f, %f, %f, %f ... %f, %f, %f, %f\n", result.vertpos[0], result.vertpos[1], result.vertpos[2], result.vertpos[3], result.vertpos[pblocksize - 4], result.vertpos[pblocksize - 3], result.vertpos[pblocksize - 2], result.vertpos[pblocksize - 1]);   
		#endif
	}

	// Face nromalssss

	if (facenorpresent == 1) {
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 'n') {
				break;
			}
		}
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		int seekback = ftello(liymfile) - 1;
		fseek(liymfile, seekback, SEEK_SET);

		int nblocksize = 0;
		int nmaxfloatsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				nmaxfloatsize = liym_max(i, nmaxfloatsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
				
				nblocksize++;
			}
		}
		
		if(nblocksize / 3 != tricount) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### N (face) block size inconsistent with triangle count.\n");
			#endif
			result.filetype = 0;
			return result;
		}

	// Read values from n block

		result.facenor = malloc(nblocksize * sizeof(float));
	
		fseek(liymfile, seekback - 1, SEEK_SET);
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
		fseek(liymfile, -1L, SEEK_CUR);
		//Should be on the carriage return now


		char *tempfloat;
		tempfloat = malloc(nmaxfloatsize * sizeof(char));
		for(int b = 0; b < nmaxfloatsize; b++) {
			tempfloat[b] = 'a';
		}	

		{		
			int j = 0;
			while (liym_isAny(cur)) {
				for(int b = 0; b < nmaxfloatsize; b++) {
					tempfloat[b] = 'a';
				}
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					tempfloat[i] = cur;
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					//printf("%s\n", tempfloat);
					result.facenor[j] = atof(tempfloat);
				} else {
					break;
				}
				j++;
			}
		}
		free(tempfloat);

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("n block read. sample:\n     %f, %f, %f, %f ... %f, %f, %f, %f\n", result.facenor[0], result.facenor[1], result.facenor[2], result.facenor[3], result.facenor[nblocksize - 4], result.facenor[nblocksize - 3], result.facenor[nblocksize - 2], result.facenor[nblocksize - 1]);   
		#endif
	}

	// Vertex normals.

	if (vertnorpresent == 1) {
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 'n') {
				break;
			}
		}
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		int seekback = ftello(liymfile) - 1;
		fseek(liymfile, seekback, SEEK_SET);

		int nblocksize = 0;
		int nmaxfloatsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				nmaxfloatsize = liym_max(i, nmaxfloatsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
				
				nblocksize++;
			}
		}
		
		if(nblocksize / 9 != tricount) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### N (vert) block size inconsistent with triangle count.\n");
			#endif
			result.filetype = 0;
			return result;
		}

	// Read values from n block

		result.vertnor = malloc(nblocksize * sizeof(float));
	
		fseek(liymfile, seekback - 1, SEEK_SET);
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
		fseek(liymfile, -1L, SEEK_CUR);
		//Should be on the carriage return now

		char *tempfloat;
		tempfloat = malloc(nmaxfloatsize * sizeof(char));
		for(int b = 0; b < nmaxfloatsize; b++) {
			tempfloat[b] = 'a';
		}	

		{		
			int j = 0;
			while (liym_isAny(cur)) {
				for(int b = 0; b < nmaxfloatsize; b++) {
					tempfloat[b] = 'a';
				}
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					tempfloat[i] = cur;
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					//printf("%s\n", tempfloat);
					result.vertnor[j] = atof(tempfloat);
				} else {
					break;
				}
				j++;
			}
		}
		free(tempfloat);

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("n block read. sample:\n     %f, %f, %f, %f ... %f, %f, %f, %f\n", result.vertnor[0], result.vertnor[1], result.vertnor[2], result.vertnor[3], result.vertnor[nblocksize - 4], result.vertnor[nblocksize - 3], result.vertnor[nblocksize - 2], result.vertnor[nblocksize - 1]);   
		#endif
	}

	// texcoordsss

	if (texcordpresent == 1) {
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 't') {
				break;
			}
		}
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		int seekback = ftello(liymfile) - 1;
		fseek(liymfile, seekback, SEEK_SET);

		int tblocksize = 0;
		int tmaxfloatsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				tmaxfloatsize = liym_max(i, tmaxfloatsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
				
				tblocksize++;
			}
		}
		
		//printf("tblocksize %d, tricount %d", tblocksize, tricount);

		if(tblocksize / 6 != tricount) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### t block size inconsistent with triangle count.\n");
			#endif
			result.filetype = 0;
			return result;
		}

	// Read values from t block

		result.texcord = malloc(tblocksize * sizeof(float));
	
		fseek(liymfile, seekback - 1, SEEK_SET);
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
		fseek(liymfile, -1L, SEEK_CUR);
		//Should be on the carriage return now


		char *tempfloat;
		tempfloat = malloc(tmaxfloatsize * sizeof(char));
		for(int b = 0; b < tmaxfloatsize; b++) {
			tempfloat[b] = 'a';
		}	

		{		
			int j = 0;
			while (liym_isAny(cur)) {
				for(int b = 0; b < tmaxfloatsize; b++) {
					tempfloat[b] = 'a';
				}
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					tempfloat[i] = cur;
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					//printf("%s\n", tempfloat);
					result.texcord[j] = atof(tempfloat);
				} else {
					break;
				}
				j++;
			}
		}
		free(tempfloat);

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("t block read. sample:\n     %f, %f, %f, %f ... %f, %f, %f, %f\n", result.texcord[0], result.texcord[1], result.texcord[2], result.texcord[3], result.texcord[tblocksize - 4], result.texcord[tblocksize - 3], result.texcord[tblocksize - 2], result.texcord[tblocksize - 1]);   
		#endif
	}

	// Search for c block

	if (vcindexpresent == 1) {
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 'c') {
				break;
			}
		}
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		int seekback = ftello(liymfile) - 1;
		fseek(liymfile, seekback, SEEK_SET);

		int cblocksize = 0;
		int cmaxintsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
				}
				cmaxintsize = liym_max(i, cmaxintsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
				
				cblocksize++;
			}
		}
		
		if(cblocksize / 3 != tricount) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### c block size inconsistent with triangle count.\n");
			#endif
			result.filetype = 0;
			return result;
		}

	// Read values from c block

		result.vcindex = malloc(cblocksize * sizeof(int));
	
		fseek(liymfile, seekback - 1, SEEK_SET);
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
		fseek(liymfile, -1L, SEEK_CUR);
		//Should be on the carriage return now


		char *tempint;
		tempint = malloc(cmaxintsize * sizeof(char));
		for(int b = 0; b < cmaxintsize; b++) {
			tempint[b] = 'a';
		}	

		{		
			int j = 0;
			while (liym_isAny(cur)) {
				for(int b = 0; b < cmaxintsize; b++) {
					tempint[b] = 'a';
				}
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					tempint[i] = cur;
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					//printf("%s\n", tempfloat);
					result.vcindex[j] = atoi(tempint);	
				} else {
					break;
				}
				j++;
			}
		}
		free(tempint);

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("c block read. sample:\n     %d, %d, %d, %d ... %d, %d, %d, %d\n", result.vcindex[0], result.vcindex[1], result.vcindex[2], result.vcindex[3], result.vcindex[cblocksize - 4], result.vcindex[cblocksize - 3], result.vcindex[cblocksize - 2], result.vcindex[cblocksize - 1]);   
		#endif
	}

	// Search for c block

	if (vertcolpresent == 1) {
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 'c') {
				break;
			}
		}
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		int seekback = ftello(liymfile) - 1;
		fseek(liymfile, seekback, SEEK_SET);

		int cblocksize = 0;
		int cmaxintsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
				}
				cmaxintsize = liym_max(i, cmaxintsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
				
				cblocksize++;
			}
		}
		
		if(cblocksize / 9 != tricount) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### c block size inconsistent with triangle count.\n");
			#endif
			result.filetype = 0;
			return result;
		}

	// Read values from c block

		result.vertcol = malloc(cblocksize * sizeof(int));
	
		fseek(liymfile, seekback - 1, SEEK_SET);
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
		fseek(liymfile, -1L, SEEK_CUR);
		//Should be on the carriage return now


		char *tempint;
		tempint = malloc(cmaxintsize * sizeof(char));
		for(int b = 0; b < cmaxintsize; b++) {
			tempint[b] = 'a';
		}	

		{		
			int j = 0;
			while (liym_isAny(cur)) {
				for(int b = 0; b < cmaxintsize; b++) {
					tempint[b] = 'a';
				}
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					tempint[i] = cur;
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					//printf("%s\n", tempfloat);
					result.vertcol[j] = atoi(tempint);	
				} else {
					break;
				}
				j++;
			}
		}
		free(tempint);

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("c block read. sample:\n     %d, %d, %d, %d ... %d, %d, %d, %d\n", result.vertcol[0], result.vertcol[1], result.vertcol[2], result.vertcol[3], result.vertcol[cblocksize - 4], result.vertcol[cblocksize - 3], result.vertcol[cblocksize - 2], result.vertcol[cblocksize - 1]);   
		#endif
	}

	// Search for w block

	if (mtxidxpresent == 1) {
		while ((cur = fgetc(liymfile)) != EOF) {
			if (cur == 'w') {
				break;
			}
		}
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something

		int seekback = ftello(liymfile) - 1;
		fseek(liymfile, seekback, SEEK_SET);

		int wblocksize = 0;
		int wmaxintsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
				}
				wmaxintsize = liym_max(i, wmaxintsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
				
				wblocksize++;
			}
		}
		
		if(wblocksize / 3 != tricount) {
			#ifdef LIYM_ALLOW_ERROR_PRINTING
				printf("### c block size inconsistent with triangle count.\n");
			#endif
			result.filetype = 0;
			return result;
		}

	// Read values from w block

		result.mtxidx = malloc(wblocksize * sizeof(int));
	
		fseek(liymfile, seekback - 1, SEEK_SET);
		while (!liym_isAlphaNumeric(cur = fgetc(liymfile))) {} //Seeks past a carriage return or something
		fseek(liymfile, -1L, SEEK_CUR);
		//Should be on the carriage return now


		char *tempint;
		tempint = malloc(wmaxintsize * sizeof(char));
		for(int b = 0; b < wmaxintsize; b++) {
			tempint[b] = 'a';
		}	

		{		
			int j = 0;
			while (liym_isAny(cur)) {
				for(int b = 0; b < wmaxintsize; b++) {
					tempint[b] = 'a';
				}
				int i = 0;
				while ((cur = fgetc(liymfile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					tempint[i] = cur;
					i++;
					if(cur == 255 || cur == EOF) { break; }
				}
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					//printf("%s\n", tempfloat);
					result.mtxidx[j] = atoi(tempint);	
				} else {
					break;
				}
				j++;
			}
		}
		free(tempint);

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("w block read. sample:\n     %d, %d, %d, %d ... %d, %d, %d, %d\n", result.mtxidx[0], result.mtxidx[1], result.mtxidx[2], result.mtxidx[3], result.mtxidx[wblocksize - 4], result.mtxidx[wblocksize - 3], result.mtxidx[wblocksize - 2], result.mtxidx[wblocksize - 1]);   
		#endif
	}

	fclose(liymfile);
	return result;
}

void freeLiym(struct liymParseResults results) {
	if((int8_t)results.filetype & 0b00000001) {free(results.vertpos);};
	if((int8_t)results.filetype & 0b00000010) {free(results.facenor);};
	if((int8_t)results.filetype & 0b00000100) {free(results.vertnor);};
	if((int8_t)results.filetype & 0b00001000) {free(results.texcord);};
	if((int8_t)results.filetype & 0b00010000) {free(results.vcindex);};
	if((int8_t)results.filetype & 0b00100000) {free(results.vertcol);};
	if((int8_t)results.filetype & 0b01000000) {free(results.mtxidx );};
}



//	#	#	#	#	#	#	#	#	Liya



struct liyaParseResults {
	float *framedata;
	int framecount;
	int currentframe;
	int streamingcur;
	int datastart;
	int bonecount;
	int maxwordsize;
};

void printLaPR(struct liyaParseResults a) {
	printf("\nFramedata is at %d\nFramecount is %d\nCurrentframe is %d\nStreamingcur is %d\nDatastart is %d\nBonecount is %d\nMaxwordsize is %d\n", a.framedata, a.framecount, a.currentframe, a.streamingcur, a.datastart, a.bonecount, a.maxwordsize);
}

struct liyaParseResults initLiyaStreamer(char *infilename, FILE **liyafilep) {
	#ifdef LIYM_ALLOW_ERROR_PRINTING
		printf("Liya streamer was compiled with error printing enabled\n");
	#endif
	#ifdef LIYM_ALLOW_INFO_PRINTING
		printf("Liya streamer was compiled with info printing enabled\n");
	#endif

	struct liyaParseResults result;

	char cur;

	*liyafilep = fopen(infilename, "r");

	if (*liyafilep == NULL) {
		#ifdef LIYM_ALLOW_ERROR_PRINTING
			printf("### Couldn't open file\n");
		#endif
		result.framecount = 0;
		result.bonecount = 0;
		return result;
	}

	FILE *liyafile = *liyafilep;

	fseek(liyafile, 0, SEEK_SET);
	cur = fgetc(liyafile);

	// Find bone count

		unsigned int bonecount = 0; //Not bonecount just general primcount. done to abstract away bone so liya can stream arbitrary data

		{
			char bonecountstr[9] = "aaaaaaaaa";
			bonecountstr[0] = cur;
			int i = 1;
			while (liym_isAlphaNumeric(cur = fgetc(liyafile))) {
				bonecountstr[liym_min(i, 8)] = cur;
				i++;
			}

			bonecount = atoi(bonecountstr);
		}
			
		result.bonecount = bonecount;

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("If interpreted as bones the animation primitive count is %d\n", bonecount / 9);
		#endif

		while (!liym_isAlphaNumeric(cur = fgetc(liyafile))) {} //Seeks past a carriage return or something

	//Find frame count

		unsigned int framecount = 0;

		{
			char framecountstr[10] = "aaaaaaaaaa";
			framecountstr[0] = cur;
			int i = 1;
			while (liym_isAlphaNumeric(cur = fgetc(liyafile))) {
				framecountstr[liym_min(i, 9)] = cur;
				i++;
			}

			framecount = atoi(framecountstr);
		}
			
		result.framecount = framecount;

		#ifdef LIYM_ALLOW_INFO_PRINTING
			printf("Frame count is %d\n", framecount);
		#endif

		while (!liym_isAlphaNumeric(cur = fgetc(liyafile))) {} //Seeks past a carriage return or something

	//prepare framedata 
	result.framedata = malloc(sizeof(float) * bonecount);

	//cursor is on the first character of the streamable (a - or a 0 or something)

	result.streamingcur = ftello(liyafile);
	result.datastart = ftello(liyafile);
	result.currentframe = 0;

	//see size of datablock

	while (!liym_isAlphaNumeric(cur = fgetc(liyafile))) {} //Seeks past a carriage return or something

		fseek(liyafile, result.datastart - 1, SEEK_SET);

		int amaxfloatsize = 0;
		{		
			while (liym_isAny(cur)) {
				int i = 0;
				while ((cur = fgetc(liyafile)) != EOF) {
					if (!liym_isAlphaNumeric(cur)) {
						break;
					}
					//printf("\"%c\", %d\n", cur, i);
					i++;
				}
				amaxfloatsize = liym_max(i, amaxfloatsize);
				if (cur == ' ') { //Spaces should be the only character separating parts of the array
					
				} else {
					break;
				}
			}
		}
		
	result.maxwordsize = amaxfloatsize;
	//printf("Max word size is %d\n", amaxfloatsize);

	//fclose(liyafile);
	return result;
}

struct liyaParseResults liyaResetStream(struct liyaParseResults activeparse) {
	struct liyaParseResults result = activeparse;
	result.streamingcur = result.datastart;
	result.currentframe = 0;
	return result;
}

struct liyaParseResults liyaStreamframe(struct liyaParseResults activeparse, FILE *liyafile) {
	struct liyaParseResults result = activeparse;
	result.currentframe++;

	if(result.currentframe > result.framecount) {
		#ifdef LIYM_ALLOW_ERROR_PRINTING
			printf("Requested stream frame is past frame data, leaving frame data unchanged\n");
		#endif
		result.currentframe = result.framecount;
		return result;
	}

	//printf("%d\n", result.currentframe);

	if (liyafile == NULL) {
		#ifdef LIYM_ALLOW_ERROR_PRINTING
			printf("### Please help.\n");
		#endif
		return result;
	}


	char cur;

	fseek(liyafile, result.streamingcur - 1, SEEK_SET);
	cur = fgetc(liyafile);

	char *tempfloat;
	tempfloat = malloc(result.maxwordsize * sizeof(char));
	for(int b = 0; b < result.maxwordsize; b++) {
		tempfloat[b] = 'a';
	}	

	{		
		int j = 0;
		while (liym_isAny(cur)) {
			if(j >= result.bonecount) {
				break;
			}

			for(int b = 0; b < result.maxwordsize; b++) {
				tempfloat[b] = 'a';
			}
			int i = 0;
			while ((cur = fgetc(liyafile)) != EOF) {
				if (!liym_isAlphaNumeric(cur)) {
					break;
				}
				//printf("\"%c\", %d\n", cur, i);
				tempfloat[i] = cur;
				i++;
				if(cur == 255 || cur == EOF) { break; }
			}
			if (cur == ' ') { //Spaces should be the only character separating parts of the array
				//printf("%s\n", tempfloat);
				result.framedata[j] = atof(tempfloat);	
			} else {
				break;
			}
			j++;
		}
	}
	free(tempfloat);

	#ifdef LIYA_ALLOW_STREAM_INFO_PRINTING
		printf("a block for frame %d streamed. sample:\n     %f, %f, %f, %f ... %f, %f, %f, %f\n", result.currentframe, result.framedata[0], result.framedata[1], result.framedata[2], result.framedata[3], result.framedata[(result.bonecount) - 4], result.framedata[(result.bonecount) - 3], result.framedata[(result.bonecount) - 2], result.framedata[(result.bonecount) - 1]);   
	#endif

	result.streamingcur = ftello(liyafile);

	return result;
}

void freeLiya(struct liyaParseResults a, FILE *liyafile) {
	if (a.framedata != 0) {
		free(a.framedata);
	}
	fclose(liyafile);
}


#endif