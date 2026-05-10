# Changelog

## [2.0.0] — Refactored Release

### Changed
- Merged duplicate `BoysHostel` / `GirlsHostel` logic into shared `Hostel` base class — eliminated ~150 lines of copy-pasted code
- Extracted `registerResident()` function — registration flow no longer duplicated for boys/girls
- Extracted `collectResidentDetails()` — form input collected in one place
- Extracted `hostelAdminMenu()` — admin sub-menu no longer duplicated
- Moved admin credentials to named constants (`ADMIN_USERNAME`, `ADMIN_PASSWORD`)
- Moved all fee values to named constants (`REGISTRATION_FEE`, `ROOM_PRICE_*`)
- Renamed `assignResident()` → `markOccupied()` (more accurate description)
- Renamed `editResidentData()` → `editDetails()` inside `Resident` class
- `saveToFile()` now saves to `data/` subfolder with descriptive filenames
- Output file format improved with aligned columns and section separators

### Added
- `readInt()` utility — prevents crash when user enters letters for numeric fields
- `readDouble()` utility — validates payment amounts
- `printReceipt()` function — formatted, aligned payment receipt
- `displayFullInfo()` method on `Resident` for detailed output
- Class diagram in `docs/class_diagram.md`
- `.gitignore` for C++ build artifacts

### Fixed
- Typo: `"Selcet Hostel"` → `"Select Hostel"`
- Variable shadowing: `payment` was declared twice in `main()`
- `cin.ignore()` placement inconsistencies that caused input skipping
- Compiler warnings about member initialization order in `Resident` constructor

## [1.0.0] — Original Submission

### Added
- `Resident` class with full personal/academic data
- `Room` class with occupancy tracking
- `Hostel` abstract base class with pure virtual `displayFacilities()` and `displayRules()`
- `BoysHostel` and `GirlsHostel` classes inheriting from `Hostel`
- `Payment<T>` template class for fee processing
- Registration flow for boys and girls hostels
- Admin login with username/password check
- File output to `BoysHostel_details.txt` and `GirlsHostel_details.txt`
- Console menu with facilities, rules, and admin options
