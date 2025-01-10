/**
 * @file NierHook.hpp
 * @author Asiern (asiern.dev@gmail.com)
 * @brief NieR:Automata Memory Hook
 * @version 0.1
 * @date 2023-03-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef NIERHOOK_H
#define NIERHOOK_H
#include "Offsets.hpp"
#ifdef USING_WINSOCK2
#include <WinSock2.h>
#else
#include <Windows.h>
#endif

#include <TlHelp32.h>
#include <string>
#include <vector>

#define VER_1_0_1 1
#define VER_1_0_2 2
#define AUTO -1

enum class Item : unsigned long {
    // Restore items
    SmallRecovery = 0,
    MediumRecovery = 1,
    LargeRecovery = 2,
    FullRecovery = 3,
    VisualCure = 60,
    AuralCure = 70,
    CureManipulation = 75,
    CureAllStatus = 80,
    CureAllHealAll = 90,

    // Material
    Amber = 633,
    AncientMask = 682,
    BeastHide = 510,
    BlackPearl = 639,
    BoarMeat = 517,
    BrokenBattery = 545,
    BrokenCircuit = 646,
    BrokenKey = 540,
    CopperOre = 600,
    CleanNut = 560,
    ComplexGadget = 582,
    CrushedNut = 636,
    Crystal = 680,
    DrabBracelet = 641,
    DentedPlate = 610,
    DentedSocket = 645,
    Dye = 631,
    EagleEggs = 638,
    ElaborateGadget = 581,
    FillerMetal = 630,
    GiantEgg = 637,
    GoldOre = 603,
    IronOre = 601,

    Unknown = 9999,
};

typedef struct
{
    Item id;
    int quantity;
} inventoryItem;

typedef struct
{
    int id;
    short level;
} inventoryWeapon;

class NieRHook;
class NieRHook
{
  private:
    // Attributes
    DWORD _pID;
    uintptr_t _baseAddress;
    uintptr_t _entityAddress;
    bool _hooked;
    offsets _offsets;

    // Player Attributes
    int Level;
    int Health;
    int MaxHealth;
    float Xpos;
    float Ypos;
    float Zpos;
    int Funds;
    int EXP;
    // int version;

    // Methods
    DWORD _getProcessID(void);
    uintptr_t _getModuleBaseAddress(DWORD procId, const wchar_t* modName);
    void _patch(BYTE* destination, BYTE* src, unsigned int size);
    void _enableCheat(cheat* c);
    void _disableCheat(cheat* c);
    template <typename T> T readMemory(uintptr_t address);
    template <typename T> void writeMemory(uintptr_t address, T value);
    char* readMemoryString(uintptr_t address, int size);
    // void getGameVersion();

  public:
    NieRHook();
    ~NieRHook();
    void start(int version);
    void start(void); // Start hook
    void stop(void);  // Reset hook
    bool isHooked(void);
    void hookStatus(void);
    // int getVersion(void);
    bool isSavefileLoaded(void);

    DWORD getProcessID(void); // Returns process id

    void update(void); // Update Player Attributes

    // Getters
    int getLevel(void);
    int getHealth(void);
    int getMaxHealth(void);
    int getFunds(void);
    int getEXP(void);
    float getXPosition(void);
    float getYPosition(void);
    float getZPosition(void);

    // Setters
    void setX(float X);
    void setY(float Y);
    void setZ(float Z);
    void setPosition(float X, float Y, float Z);
    void setHealth(int health);
    void setFunds(int funds);

    // Cheats
    void NoCLip(bool enabled);
    void InfiniteDoubleJump(bool enabled);
    void setGameSpeed(float speed);
    void NoCooldown(bool enabled);
    void InfiniteAirDash(bool enabled);
    void IgnoreUpgradeMaterials(bool enabled);
    void FreeCamera(bool enabled);
    void InfiniteItemUsage(bool enabled);
    void SetA2Dash(bool enabled);
    void Set2BDash(bool enabled);
    void InfiniteBuffDuration(bool enabled);
    void SaveAnywhere(bool enabled);
    void InfiniteConsumableItems(bool enabled);

    // Inventory
    bool addItem(int ID, int number);
    bool removeItem(int ID);
    bool addWeapon(int ID, int level);
    bool removeWeapon(int ID);
    std::vector<inventoryItem> readInventory(void);
    std::vector<inventoryWeapon> readWeapons(void);

    // Misc
    void setMusicVolume(int value);
    void setSoundEffectVolume(int value);
    void setVoiceVolume(int value);
    void setAudioOutput(int value);
    void setVoiceChanger(int value);
    void setBrightness(int value);
    void setDistance(int value);
    void setCombatDistance(int value);
    void setVerticalRotationSpeed(int value);
    void setHorizontalRotationSpeed(int value);
    void setHorizontalAutoAdjust(int value);
    void setVerticalAutoAdjust(int value);
    void setFreeEnemyTracking(int value);
    void setZoomSpeed(int value);
    void setPursuitSpeed(int value);
    void setLockedEnemyTracking(int value);
    void setHUDOpacity(float opacity);
    void setColor(float R, float G, float B);
    int getMusicVolume();
    int getSoundEffectVolume();
    int getVoiceVolume();
    int getAudioOutput();
    int getVoiceChanger();
    int getBrightness();
    int getDistance();
    int getCombatDistance();
    int getVerticalRotationSpeed();
    int getHorizontalRotationSpeed();
    int getHorizontalAutoAdjust();
    int getVerticalAutoAdjust();
    int getFreeEnemyTracking();
    int getZoomSpeed();
    int getPursuitSpeed();
    int getLockedEnemyTracking();
    float getHUDOpacity();
};

template <typename T> inline T NieRHook::readMemory(uintptr_t address)
{
    T value;
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->_pID);
    ReadProcessMemory(pHandle, (LPCVOID)(address), &value, sizeof(value), NULL);
    CloseHandle(pHandle);
    return value;
}

template <typename T> inline void NieRHook::writeMemory(uintptr_t address, T value)
{
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->_pID);
    WriteProcessMemory(pHandle, (LPVOID)(address), &value, sizeof(value), NULL);
    CloseHandle(pHandle);
}

#endif