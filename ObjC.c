/*
    The scope of this file is to demonstrate OOP implementation in C, including
    simple object inheritance.
    I actually didn't try to build this since I'm lazy to set up the environment,
    there might be some mistakes, but the general idea shall be clear.

    All the structures of the objects are kept inside the implementation (*.c) file,
    thus they are not exposed to the user of the library, he only gets something
    called "opaque pointer", pretty much a handle typedefed to the base class.
    The content of the object pointed to by the opaque pointer can't be interpreted
    without the knowledge of the objects internal structure -> encapsulation.
*/

/********************************************************************************
*   INCLUDES                                                                    *
********************************************************************************/

// not sure if all are necessary
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "objC.h"
/********************************************************************************
*  CONTENT OF objC.h HEADER FILE                                                *
********************************************************************************/
#ifndef OBJ_C_H
#define OBJ_C_H

typedef enum
{
    VEH_TYPE_PASSENGER = 0,
    VEH_TYPE_VAN,
    VEH_TYPE_TRUCK,
    VEH_TYPE_CNT    // keep this at the end of the struct
} vehicleType_t;

typedef bool (*vehicleDrive_fp)(void);
typedef bool (*destructor_fp)(void);

/* this is the tricky part I always need to google and I'm not sure I did it right this time, since I'm bloody lazy... The thing is:
    - you need to have some symbolic name (hence the first typedef) for the base struct even before you actually define the base structure itself,
      so you can then reference itself ("this") as the first member of the base. The trick consisted of typedef-ing the struct first and then creating
      the base struct without "typedef" as you're used to (see the passengerCsarClass_t and vanClass_t for reference) */
typedef struct base_t vehicleBase_t;

struct base_t
{
    vehicleBase_t*  basePtr;
    vehicleType_t   vehType;
    engineType_t    engType;
    vehicleDrive_fp driveMethod;
    destructor_fp   destructor;
    size_t          objSize;        // maybe we don't need to know the object size??? Keeping this for future, delete it in a week if not used...
};

vehicleBase_t* vehicleInit(vehicleType_t vehType);
bool vehicleDrive(vehicleBase_t* vehPtr);
bool vehicleDestruct(vehicleBase_t* vehPtr);

#endif

/********************************************************************************
*   TYPE DEFINITIONS                                                            *
********************************************************************************/
typedef enum
{
    ENG_TYPE_GASOLINE = 0,
    ENG_TYPE_DIESEL,
    ENG_TYPE_ELECTRO,
    ENG_TYPE_HYBRID,
    ENG_TYPE_OTHER,
    ENG_TYPE_CNT    // keep this at the end of the struct
} engineType_t;

typedef enum
{
    CARGO_TYPE_PERSON = 0,
    CARGO_TYPE_FOOD,
    CARGO_TYPE_LIVE_ANIMALS,
    CARGO_TYPE_CHEMICALS,
    CARGO_TYPE_OTHERS,
    CARGO_TYPE_CNT
} cargoType_t;

typedef struct
{
    base_t      base;
    uint8_t     nrOfSeats;
    uint16_t    bootSpace_litres;
} passengerCarClass_t;

typedef struct
{
    base_t      base;
    cargoType_t cargoType;
} vanClass_t;


/********************************************************************************
*   PRIVATE METHODS AND VARIABLES                                               *
********************************************************************************/

// declaration of "drive" methods conforming the vehicleDrive_fp signature
static bool drivePassCar(void);
static bool driveVan(void);
static bool driveTruck(void);

// declaration of destructor methods
static bool destrPassCar(void);
static bool destrVan(void);
static bool destrTruck(void);

// keep the order the same as in vehicleType_t type definition...
static const vehicleDrive_fp driveMethods[] = {drivePassCar, driveVan, driveTruck};
static const destructor_fp destructors[] = {destrPassCar, destrVan, destrTruck};


/********************************************************************************
*   PUBLIC METHODS DEFINITIONS                                                  *
********************************************************************************/

// returns an opaque pointer to a new vehicle, NULL on failure
vehicleBase_t* vehicleInit(vehicleType_t vehType)
{
    base_t* ret = NULL;
    size_t objSize = 0;

    switch(vehType)
    {
        case VEH_TYPE_PASSENGER:
            objSize = sizeof(passengerCarClass_t);
            ret = (vehicleBase_t*)malloc(passengerCarClass_t);
            ret->destructor = 
            break;

        case VEH_TYPE_VAN:
            {
                objSize = sizeof(vanClass_t);
                vanClass_t van = malloc(objSize);
                van->base->engType = ENG_TYPE_DIESEL;       // we only support diesel vans now...
                ret = (vehicleBase_t*)van;
            }
            break;

        case VEH_TYPE_TRUCK:
        default:
            break;
    }

    if(objSize)
    {
        ret->objSize = objSize;
    }
    else
    {
        ret = NULL;
    }
    
    if(ret)
    {
        // configure the correct methods
        ret->driveMethod = driveMethods[vehType];
        ret->destructor  = destructors[vehType];
    }

    return ret;
}

// returns false on failure
bool vehicleDrive(vehicleBase_t* vehPtr)
{
    bool ret = false;
    
    if((vehPtr) && (VEH_TYPE_CNT > vehPtr->vehType) && (vehPtr->driveMethod))
    {
        ret = vehPtr->driveMethod();
    }

    return ret;
}

// returns false on failure
bool vehicleDestruct(vehicleBase_t* vehPtr)
{
    ret = false;

    if((vehPtr) && (VEH_TYPE_CNT > vehPtr->vehType) && (vehPtr->destructor))
    {
        ret = vehPtr->destructor();
    }
    
    return ret;
}
            
