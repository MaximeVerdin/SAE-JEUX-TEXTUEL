#include "upgrade.h"

typedef struct 
{
    Upgrade upgrade;
    struct UpgradeNode *left;
    struct UpgradeNode *right;
} UpgradeNode;
