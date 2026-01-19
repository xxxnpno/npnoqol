## Work in Progress

### Current Development
- Hooking method arguments via **Just-In-Time compilation** and forcing their return values  
  - enables creation of new commands by intercepting `addChatMessage`
  - creation of CameraNoClip module by force return `rayTraceBlocks` if called by `orientCamera`, requires `jvmti->GetStackTrace` but trivial

## Features

### Hypixel
- **Nick Alert**
- **Auto GG**  
  *(Currently supported only for Blitz Survival Games)*

### Blitz Survival Games
- **Tab List Format** 
```
[<wins>] <rank> <name> <hp>
```
- **Nametag Format**
```
<name> <hp>
```