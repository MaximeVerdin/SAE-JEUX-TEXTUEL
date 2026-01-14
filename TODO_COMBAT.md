# TODO: Combat System Update - Fight & Block

## Objective

Implement a combat system where the player can choose to "fight" or "block" during combat encounters, with luck-based critical hits and parries.

## Tasks Completed ✓

### 1. Modify combat() function in playGame.c ✓

- [x] Add combat action menu with "1. Fight" and "2. Block" options
- [x] Implement "Fight" action (attack mechanic)
- [x] Implement "Block" action (reduce incoming damage by 50%)
- [x] Add blocking state tracking between turns
- [x] Add critical hit system based on luck
- [x] Add parry system based on luck

### 2. Update combat flow ✓

- [x] Player chooses action each turn
- [x] "Fight": Deal damage to enemy (with critical hit chance)
- [x] "Block": Take 50% reduced damage (with parry chance)
- [x] Display combat status each turn
- [x] Display luck stat to player during combat

## Luck-Based Mechanics

### Critical Hits (Fighting):

- **Base chance**: 15%
- **Bonus per luck point**: +2%
- **Formula**: `critChance = 15 + (player->luck * 2)`
- **Effect**: Double damage on critical hit

### Parry (Blocking):

- **Base chance**: 10%
- **Bonus per luck point**: +3%
- **Formula**: `parryChance = 10 + (player->luck * 3)`
- **Effect**:
  - No damage taken
  - Counter-attack deals damage to enemy

## New functions added:

- `displayCombatMenu()` - Shows combat action options

## Modified functions:

- `combat()` - Now includes player decision-making, critical hits, and parries
