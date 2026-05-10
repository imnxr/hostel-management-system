# Class Diagram — Hostel Management System

## Inheritance Hierarchy

```
          ┌─────────────────────────────┐
          │         Hostel              │  <<Abstract>>
          │─────────────────────────────│
          │ # hostelName : string       │
          │ # dataFile : string         │
          │ # residents : vector        │
          │ # rooms : vector            │
          │─────────────────────────────│
          │ + findAvailableRoom()       │
          │ + addResident()             │
          │ + showAvailableRooms()      │
          │ + showBookedRooms()         │
          │ + showResidentsData()       │
          │ + editResidentData()        │
          │ + saveToFile()              │
          │ + displayFacilities() = 0   │  <<pure virtual>>
          │ + displayRules() = 0        │  <<pure virtual>>
          └──────────────┬──────────────┘
                         │
            ┌────────────┴────────────┐
            │                         │
┌───────────▼──────────┐  ┌──────────▼───────────┐
│     BoysHostel       │  │     GirlsHostel       │
│──────────────────────│  │───────────────────────│
│ rooms: B1–B200       │  │ rooms: G1–G200        │
│──────────────────────│  │───────────────────────│
│ + displayFacilities()│  │ + displayFacilities() │
│ + displayRules()     │  │ + displayRules()      │
└──────────────────────┘  └───────────────────────┘
```

## Other Classes

```
┌──────────────────────────────────┐
│            Resident              │
│──────────────────────────────────│
│ - name : string                  │
│ - fatherName : string            │
│ - regNumber : string             │
│ - cnic : string                  │
│ - phone : string                 │
│ - age : int                      │
│ - department : string            │
│ - semester : int                 │
│ - address : string               │
│ - roomNumber : string            │
│──────────────────────────────────│
│ + getters (all fields)           │
│ + setRoomNumber()                │
│ + displayInfo()                  │
│ + displayFullInfo()              │
│ + editDetails()                  │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│              Room                │
│──────────────────────────────────│
│ - roomNumber : string            │
│ - isOccupied : bool              │
│──────────────────────────────────│
│ + markOccupied()                 │
│ + getRoomNumber()                │
│ + getIsOccupied()                │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│         Payment<T>               │  <<Template>>
│──────────────────────────────────│
│ + registrationFee : T            │
│──────────────────────────────────│
│ + getRoomPrice(type) : T         │
│ + processRegistrationPayment()   │
│ + processRoomPayment(price)      │
└──────────────────────────────────┘
```

## Key Relationships

- `Hostel` **has-many** `Room` (composition — rooms live inside the hostel)
- `Hostel` **has-many** `Resident` (aggregation — residents are added over time)
- `BoysHostel` / `GirlsHostel` **is-a** `Hostel` (inheritance)
- `Payment<T>` is independent — used inside the registration flow in `main()`
