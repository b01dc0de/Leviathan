#include <cstdio>

int main(int argc, const char* argv[])
{
	printf("BEGIN HelloTriangle:\n");
	for (int i = 1; i < argc; i++)
	{
		printf("\tArg %d: %s\n", i, argv[i]);
	}
	printf("END HelloTriangle\n");
}