#ifndef UTILS_H
#define UTILS_H

char *readParam(const char *filePath, const char *paramName);
void freeParams(char **params, int count);

/* Tutorial system functions */
char *readTutorialFile(const char *filePath);
char *getTutorialSegment(const char *tutorialContent, const char *marker);
void displayTutorialSegment(const char *tutorialContent, const char *marker);
void runTutorial(const char *language, int *tutorialProgress, int step);
int shouldShowTutorialStep(int *progress, int step);
void advanceTutorialStep(int *progress, int step);

#endif
