/* Author names : VERDIN Maxime and FOURNIER Nathan */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saveManagement.h"

char *readParam(const char *filePath, const char *paramName);
void freeParams(char **params, int count);

/**
 * @brief Get the current language code
 *
 * @return Current language code (e.g., "fr-fr", "en-gb")
 */
const char *getLanguage();

/**
 * @brief Set the current language code
 *
 * @param lang Language code to set (e.g., "fr-fr", "en-gb")
 */
void setLanguage(const char *lang);

/**
 * @brief Get translated text from trad.txt
 *
 * Looks up the text key in the current language's
 * trad.txt file and returns the translated text.
 *
 * @param textKey The key to look up in trad.txt
 * @return Dynamically allocated translated text, or copy of textKey if not found
 */
char *getTranslatedText(const char *textKey);

/**
 * @brief Get translated weapon name from trad.txt
 *
 * Looks up the weapon's internal name in the current language's
 * trad.txt file and returns the translated display name.
 *
 * @param weaponName Internal weapon name (e.g., "rusty_sword")
 * @return Dynamically allocated translated name, or copy of weaponName if not found
 */
char *getWeaponTranslatedName(const char *weaponName);

/**
 * @brief Display the tutorial at game start
 *
 * Reads and prints the tutorial content from the appropriate
 * language file based on current language setting.
 */
void displayTutorial();

/**
 * @brief Load history chapter lines from history file
 *
 * Parses the history file and extracts all lines starting with '!'
 * (chapter titles) and stores them in the game state.
 *
 * @param game Pointer to the game state to populate with history lines
 * @param storyNumber Story file number to load (1 to TOTAL_STORIES)
 */
void loadGameHistory(GameState *game, int storyNumber);

/**
 * @brief Display the next history line based on player outcome
 *
 * When player exits successfully, displays the actual history line.
 * When player dies, displays "???" to hide the line.
 *
 * @param game Pointer to the game state
 * @param playerSurvived 1 if player survived/exited successfully, 0 if died
 */
void displayHistoryLine(GameState *game, int playerSurvived);

#endif
