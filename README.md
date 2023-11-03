# Aurc
Fast & Easy way to Install/Update Aur and Non-Aur Arch Linux Packages!

### UNDER DEVELOPMENT
   - Still an alpha level code and udner heavy development for improvements and bug fixes and security concerns improvements.

## TODO

- [x] Complete the essential pacman functions
- [x] Ability to modify arch mirrorlist
- [x] Implement aur helper
- [x] Better inspection and secure aur package installation
- [ ] **Implementaion of libalpm instead of relying on pacman** (requires careful attention)
- [ ] USe internel libraries for some dependencies (json, curl etc...)
- [ ] Reflector like easy mirror change functions
- [ ] Cleaning of probable messy code
- [ ] Better optimization
- [ ] More modularity of functions

## Requirements

* Dependencies -

   - jq
   - less
   - pacman
   - curl
   - base-devel

## Usage

  * Actions :
    - update        - ( Update outdated system/user packages )
    - refresh       - ( Refresh Repository Database )
    - install       - ( Install a package )
    - install-local - ( Install a local package )
    - install-aur   - ( Install aur package )
    - install-force - ( Forcefully install a package )
    - modify-repo   - ( Modify arch repositories )
    - query         - ( Query if a package is installed )
    - search        - ( Search for a package in the base repository )
    - search-aur    - ( Search for a package in the aur repository )
    - remove        - ( Remove a package )
    - remove-dep    - ( Remove a package along with its dependencies )
    - remove-force  - ( Forcefully remove a package even if other packages depend on it )
    - remove-orp    - ( Remove orphan packages )
    
  * Options :
    - --version, -v - ( Display the version of the package manager )
    - --help,    -h - ( Display this help guide )

## Install/Update

  * Install dependencies : 

      ```bash
      sudo pacman -S jq less curl gcc pacman make base-devel
      ```

  * Install aurc : 

      ```bash
      git clone https://github.com/rifsxd/aurc.git
      ```
      ```bash
      cd aurc/src
      ```
      ```bash
      sudo make install
      ```
      
  * Update aurc : 

      ```bash
      git fetch & git pull
      ```
      ```bash
      sudo make clean install
      ```
