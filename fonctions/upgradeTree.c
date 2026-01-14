#include <stdio.h>
#include <stdlib.h>
#include "upgradeTree.h"

/**
 * @brief Create upgrade tree node
 */
UpgradeNode *createUpgradeNode(Upgrade upgrade)
{
    UpgradeNode *node = malloc(sizeof(UpgradeNode));
    if (node != NULL)
    {
        node->upgrade = upgrade;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

/**
 * @brief Free upgrade tree memory
 */
void freeUpgradeTree(UpgradeNode *root)
{
    if (root == NULL)
        return;
    freeUpgradeTree(root->left);
    freeUpgradeTree(root->right);
    free(root);
}

/**
 * @brief Display all upgrades in tree
 */
void displayUpgradeTree(UpgradeNode *root)
{
    if (root == NULL)
        return;
    displayUpgrade(&root->upgrade);
    printf("---\n");
    displayUpgradeTree(root->left);
    displayUpgradeTree(root->right);
}
