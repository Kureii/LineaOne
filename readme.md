# LineaOne

Specialized software for creating timelines for presentations.

This app is under development, the current state is very experimental and the code is not pretty

## Dependencies:

- SDL3

## Known issues
- if two headings are in the same year, they overlap
- diacritics do not appear
- untreated document reopening (same name just rewrite document)
- displaying files that are not `*.jsonlo` and are not folders
- showing hidden folders
- no checking of `*.jsonlo` data

## Road map

### Version 0.1.0 - First Full Usable App
- [x] First full worked usable app
  - [x] Create new document
  - [x] Close document
  - [x] Popup when closing unsaved window
  - [x] Create timeline event
  - [x] Remove timeline event
  - [x] Sort timeline events
  - [x] Drag and drop move events
  - [x] Show timeline
  - [x] Save file (*.jsonlo)
  - [x] Load file (*.jsonlo)
  - [x] Export to SVG

Note: `.jsonlo` is a specific format for this software. At its base, it is a `*.json` file with a specific structure for LineaOne.

### Version 0.1.1
- [ ] Fix displaying two or more headlines in one year

### Version 0.2.x - Code Improvement (start at 07/2025)
- [ ] Refactoring code
- [ ] Clear code
- [ ] Improve memory management
- [ ] Include boost lib

### Version 0.3.x - Undo/Redo Functionality
- [ ] Add undo and redo

### Version 0.4.x - Recovery and Configuration
- [ ] Recovery saves
- [ ] Add configuration
  - [ ] Dark / Light mode
  - [ ] Autosaves
  - [ ] Timeline theme

### Version 0.5.x - Extended Functionality
- [ ] Add more formats of date
- [ ] Add raster export

### Version 0.6.x - Advanced Export Options
- [ ] More export settings
  - [ ] Wrapping by defined layout (A4, Full HD etc.)
  - [ ] Add export for web (static)

### Version 0.7.x - Interactive Web Export
- [ ] Add interactive web export (the exported site will be interactive)

### Version 0.8.x - Image Support
- [ ] Add support for including images to timeline events

### Version 0.9.x - UI Customization
- [ ] Configurable UI of LineaOne

### Version 1.0 - Release Candidate
- [ ] Final testing and bug fixes
- [ ] Documentation completion
- [ ] Performance optimization
- [ ] Prepare for official release
- [ ] Windows installation
- [ ] debian based distro package