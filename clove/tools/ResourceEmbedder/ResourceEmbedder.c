#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* openOrExit(const char* fileName, const char* mode) {
	FILE* file = fopen(fileName, mode);
	if(file == NULL) {
		perror(fileName);
		exit(EXIT_FAILURE);
	}
	return file;
}

int main(int argc, char** argv) {
	if(argc < 4) {
		fprintf(stderr, "USAGE: %s \n\n", "Creates {file}.c from the contents of {res} with open type {type}\n");
		return EXIT_FAILURE;
	}

	const char* name = argv[1];
	const char* openType = argv[3];

	FILE* resource = openOrExit(argv[2], openType);

	char fileName[256];
	snprintf(fileName, sizeof(fileName), "%s.c", name);

	FILE* out = openOrExit(fileName, "w");
	fprintf(out, "#include <stdlib.h>\n\n");
	fprintf(out, "const unsigned char %s[] = {\n", name);

	unsigned char buf[256];
	size_t nread = 0;
	size_t lineCount = 0;
	do {
		nread = fread(buf, 1, sizeof(buf), resource);
		for(size_t i = 0; i < nread; ++i) {
			if(lineCount == 0) {
				fprintf(out, "\t");
			}

			fprintf(out, "0x%02X, ", buf[i]);
			
			if(++lineCount == 8) {
				fprintf(out, "\n");
				lineCount = 0;
			}
		}
	} while(nread > 0);

	if(lineCount > 0) {
		fprintf(out, "\n");
	}
	fprintf(out, "};\n\n");
	fprintf(out, "const size_t %sLength = sizeof(%s);\n", name, name);

	fclose(resource);
	fclose(out);

	return EXIT_SUCCESS;
}