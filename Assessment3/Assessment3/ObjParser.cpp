#include "ObjParser.h"
#include <iostream>
#include <fstream>

float* Verts;
float* Vt;
float* Vn;
float* f;

int v_count = 0;
int vn_count = 0;
int vt_count = 0;
int f_count = 0;

int return_value = 0;

bool prefix(const char* pre, const char* str)
{
	return strncmp(pre, str, strlen(pre)) == 0;
}

float* read_obj_file(const char* filename, float* vertices)
{
	FILE* objFile;
	errno_t err =  fopen_s(&objFile, filename, "rb");
	printf("loaded \n");
	
	char* read;
	read = (char*)malloc(3 * 100000);
	//printf("%s", read);

	char* line;
	line = (char*)malloc(3 * 100000);

	int index = 0;
	int index_vt = 0;
	int index_vn = 0;
	int index_f = 0;
	int buffer = 60;

	if (err != 0 || objFile == NULL)
	{
		printf("%s,Obj file failed to load \n", filename);
		exit(EXIT_FAILURE);
	}

	while (fgets(read, 100, objFile)) {

		if (prefix("v ", read))
		{
			read[strcspn(read, "\n")] = 10;
			read = read + 2;
			//printf("%s\n", read);
			char* read_next_token;
			char* read_token = strtok_s(read, " ", &read_next_token);
			while (read_token != NULL)
			{
				//printf("%s", token);
				v_count++;
				read_token = strtok_s(NULL, " ", &read_next_token);
			}
		}

		if (prefix("vt ", read))
		{
			read[strcspn(read, "\n")] = 10;
			read = read + 2;
			//printf("%s\n", read);
			char* read_next_token_vt;
			char* read_token_vt = strtok_s(read, " ", &read_next_token_vt);
			while (read_token_vt != NULL)
			{
				//printf("%s", token);
				vt_count++;
				read_token_vt = strtok_s(NULL, " ", &read_next_token_vt);
			}
		}

		if (prefix("vn ", read))
		{
			read[strcspn(read, "\n")] = 0;
			read = read + 2;
			//printf("%s\n", read);
			char* read_next_token_vn;
			char* read_token_vn = strtok_s(read, " ", &read_next_token_vn);
			while (read_token_vn != NULL)
			{
				//printf("%s", token);
				vn_count++;
				read_token_vn = strtok_s(NULL, " ", &read_next_token_vn);
			}
		}

		if (prefix("f ", read))
		{
			//TODO fix
			read[strcspn(read, "\n")] = 0;
			read = read + 2;
			//printf("%s\n", read);
			char* read_next_token_f;
			char* read_token_f = strtok_s(read, " ", &read_next_token_f);
			while (read_token_f != NULL)
			{
				//printf("%s", read_token_f);
				char* inner;
				
				char* face_token = strtok_s(read_token_f, "/", &inner);

				while (face_token)
				{
					//printf("%s", face_token);
					face_token = strtok_s(NULL, "/", &inner);
					f_count++;
				}

				read_token_f = strtok_s(NULL, " ", &read_next_token_f);
			}
			}
		}

	//printf("%d\n", f_count);
	//free(line);

	Verts = (float*)malloc(sizeof(float*)* (3 * v_count));
	Vt = (float*)malloc(sizeof(float*) * (3 * vt_count));
	Vn = (float*)malloc(sizeof(float*) * (3 * vn_count));

	vertices = (float*)malloc(sizeof(float*) * (f_count * 24));

	FILE* objFile_2;
	errno_t err2 = fopen_s(&objFile_2, filename, "rb");
	
	int v1, v2, v3 = 0;
	int vt1, vt2, vt3 = 0;
	int vn1, vn2, vn3 = 0;

	int face_index = 0;
	while (fgets(line, 90, objFile_2)) {

		//removes newline at the end of string
		line[strcspn(line, "\n")] = 0;
		if (prefix("v ", line))
		{
			float temp = 0;
			//printf("%s", line);

			char* newline = line + 2;
			char* next_token;
			char* token = strtok_s(newline, " ", &next_token);
			while (token != NULL) 
			{
					temp = atof(token);
					Verts[index] = temp;
					index++;

					token = strtok_s(NULL, " ", &next_token);
			}
		}
		if (prefix("vt ", line))
		{
			float temp = 0;
			//printf("%s", line);

			char* newline_vt = line + 2;
			char* next_token;
			char* token = strtok_s(newline_vt, " ", &next_token);
			while (token != NULL)
			{
				temp = atof(token);
				Vt[index_vt] = temp;
				index_vt++;
				token = strtok_s(NULL, " ", &next_token);
			}
		}
		if (prefix("vn ", line))
		{
			float temp = 0;
			//printf("%s", line);

			char* newline_vt = line + 2;
			char* next_token;
			char* token = strtok_s(newline_vt, " ", &next_token);
			while (token != NULL)
			{
				temp = atof(token);
				Vn[index_vn] = temp;
				index_vn++;

			

				token = strtok_s(NULL, " ", &next_token);
			}
		}
		if (prefix("f ", line))
		{
			//printf("%s", line);

			char* newline_f = line + 2;
			char* next_token;
			char* token = strtok_s(newline_f, " ", &next_token);
			int store = 0;

			int counter = 0;
			while (token != NULL)
			{
				
				char* inner;
				//printf("%s\n", token);
				char* face_token = strtok_s(token, "/", &inner);
				face_token[strcspn(face_token, "\n")] = 0;
				int inner_counter = 0;
				while (face_token)
				{
					store = atoi(face_token);
					//printf("%d", store);
					//prints correctly
					if (counter == 0 && inner_counter == 0)
					{
						v1 = store;
						vertices[face_index] = Verts[((v1 - 1) * 3)];
						//printf("%d index \n", ((v1 - 1) * 3));
						//printf("%f value \n", Verts[((v1 - 1) * 3)]);
						face_index++;
						vertices[face_index] = Verts[(((v1 - 1) * 3) + 1)];
						face_index++;
						//printf("%d index \n", (((v1 - 1) * 3) + 1));
						//printf("%f value \n", Verts[(((v1 - 1) * 3) + 1)]);
						vertices[face_index] = Verts[(((v1 - 1) * 3) + 2)];
						//printf("%d index \n", (((v1 - 1) * 3) + 2));
						//printf("%f value \n", Verts[(((v1 - 1) * 3) + 2)]);
						face_index++;	
						
						
					}
					if (counter == 0 && inner_counter == 1)
					{
						vt1 = store;
						vertices[face_index] = Vt[((vt1 - 1) * 2)];
						face_index++;
						vertices[face_index] = Vt[(((vt1 - 1) * 2) + 1)];
						face_index++;
					}
						
					if (counter == 0 && inner_counter == 2) 
					{
						vn1 = store;
						vertices[face_index] = Vn[((vn1 - 1) *3)];
						face_index++;
						vertices[face_index] = Vn[(((vn1 - 1) *3) + 1)];
						face_index++;
						vertices[face_index] = Vn[(((vn1 - 1) *3) + 2)];
						face_index++;
					}
					if (counter == 1 && inner_counter == 0)
					{
						v2 = store;
						vertices[face_index] = Verts[((v2 - 1) * 3)];
						face_index++;
						vertices[face_index] = Verts[(((v2 - 1) * 3) + 1)];
						face_index++;
						vertices[face_index] = Verts[(((v2 - 1) * 3) + 2)];
						face_index++;
						//printf("%d\n", v2);
					}
					if (counter == 1 && inner_counter == 1)
					{
						vt2 = store;
						vertices[face_index] = Vt[((vt2 - 1) *2)];
						face_index++;
						vertices[face_index] = Vt[(((vt2 - 1) *2) + 1)];
						face_index++;
						//printf("%d\n", vt2);
					}

					if (counter == 1 && inner_counter == 2)
					{
						vn2 = store;
						vertices[face_index] = Vn[((vn2 - 1) * 3)];
						face_index++;
						vertices[face_index] = Vn[(((vn2 - 1) * 3) + 1)];
						face_index++;
						vertices[face_index] = Vn[(((vn2 - 1) * 3) + 2)];
						face_index++;
						//printf("%d\n", vn2);
					}
					if (counter == 2 && inner_counter == 0)
					{
						v3 = store;
						vertices[face_index] = Verts[((v3 - 1) * 3)];
						face_index++;
						vertices[face_index] = Verts[(((v3 - 1) * 3) + 1)];
						face_index++;
						vertices[face_index] = Verts[(((v3 - 1) * 3) + 2)];
						face_index++;
						//printf("%d\n", v3);
					}
					if (counter == 2 && inner_counter == 1)
					{
						vt3 = store;
						vertices[face_index] = Vt[((vt3 - 1) * 2)];
						face_index++;
						vertices[face_index] = Vt[(((vt3 - 1) * 2) + 1)];
						face_index++;
						//printf("%d\n", vt3);
					}

					if (counter == 2 && inner_counter == 2)
					{
						vn3 = store;
						vertices[face_index] = Vn[((vn3 - 1) * 3)];
						face_index++;
						vertices[face_index] = Vn[(((vn3 - 1) * 3) + 1)];
						face_index++;
						vertices[face_index] = Vn[(((vn3 - 1) * 3) + 2)];
						face_index++;
						//printf("%d\n", vn3);
					}
					face_token = strtok_s(NULL, "/", &inner);
					inner_counter++;
				}
				
				token = strtok_s(NULL, " ", &next_token);
				counter++;
			}
		}
	}
	//printf("%d", index);
	//printf("%f", *Verts);
	return_value = face_index;

	for (int j = 0; j < face_index; j++)
	{
		//printf("%f\n", vertices[j]);
	}
	for (int j = 0; j < index; j++)
	{
		//printf("%f\n", Verts[j]);
		//prints correct
	}
	for (int j = 0; j < index_vn; j++)
	{
		//printf("%f\n", Vn[j]);
		//prints correctly
	}
	for (int j = 0; j < index_vt; j++)
	{
		//printf("%f\n", Vt[j]);
		//prints correctly
	}

	free(Verts);
	free(Vt);
	free(Vn);
	fclose(objFile);
	return vertices;
}

void values(int* indexes, int* f) {
	*indexes = return_value;
	*f = f_count;
}

int faces(int f)
{
	f = return_value;
	//printf("%d\n", f);
	return f;
}