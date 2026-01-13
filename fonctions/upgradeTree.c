#include <stdio.h>
#include <stdlib.h>
#include "upgradeTree.h"

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

void freeUpgradeTree(UpgradeNode *root)
{
    if (root == NULL)
        return;
    freeUpgradeTree(root->left);
    freeUpgradeTree(root->right);
    free(root);
}

void displayUpgradeTree(UpgradeNode *root)
{
    if (root == NULL)
        return;
    displayUpgrade(&root->upgrade);
    printf("---\n");
    displayUpgradeTree(root->left);
    displayUpgradeTree(root->right);
}
