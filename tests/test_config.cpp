#include "tinyfiledialogs.h"
#include <stdio.h>

int main()
{
  const char* filters[] = { "*.exe" };
  const char* selectedFile = tinyfd_openFileDialog(
      "Test Dialog",
      "C:\\",
      1,
      filters,
      "EXE Files",
      0
  );
  if (selectedFile) printf("Selected: %s\n", selectedFile);
  else printf("No file selected.\n");
  return 0;
}