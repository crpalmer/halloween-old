{{
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐               
│ SD2.0 File Allocation Table Engine                                                                                          │
│                                                                                                                             │
│ Author: Kwabena W. Agyeman                                                                                                  │                              
│ Updated: 5/27/2010                                                                                                          │
│ Designed For: P8X32A @ 80Mhz                                                                                                │
│ Version: 1.3                                                                                                                │
│                                                                                                                             │
│ Copyright (c) 2010 Kwabena W. Agyeman                                                                                       │              
│ See end of file for terms of use.                                                                                           │
│                                                                                                                             │
│ Update History:                                                                                                             │
│                                                                                                                             │
│ v1.0 - Original release - 1/7/2010.                                                                                         │
│ v1.1 - Updated everything, made code abort less, caught more errors. - 3/10/2010.                                           │
│ v1.2 - Added support for variable pin assignments and locks. - 3/12/2010.                                                   │
│ v1.3 - Improved code and added new features. - 5/27/2010.                                                                   │
│                                                                                                                             │
│ For each included copy of this object only one spin interpreter should access it at a time.                                 │
│                                                                                                                             │
│ Nyamekye,                                                                                                                   │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ SPI Circuit:                                                                                                                │
│                                                                                                                             │
│ Data Out Pin Number    ─ Data out pin. Driven by the SD card. Goes to the SD card data out pin.                           │
│                                                                                                                             │
│ Clock Pin Number       ─ Clock pin. Driven by the propeller chip. Goes to the SD clock pin.                               │
│                                                                                                                             │
│ Data In Pin Number     ─ Data in pin. Driven by the propeller chip. Goes to the SD card data in pin.                      │
│                                                                                                                             │
│ Chip Select Pin Number ─ Chip select pin. Driven by the propeller chip. Goes to the SD card chip select pin.              │
│                                                                                                                             │
│                                    3.3V                                                                                     │
│                                                                                                                            │
│                                    │                                                                                        │
│                                     10KΩ                                                                                   │
│                                    │                                                                                        │
│ Real Time Clock Data Pin Number  ─┻─ Goes to the DS1307 SDA Pin.                                                          │
│                                                                                                                             │
│                                    3.3V                                                                                     │
│                                                                                                                            │
│                                    │                                                                                        │
│                                     10KΩ                                                                                   │
│                                    │                                                                                        │
│ Real Time Clock Clock Pin Number ─┻─ Goes to the DS1307 SCL Pin.                                                          │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘ 
}}

CON

  #1, Disk_IO_Error, Clock_IO_Error
  #3, File_System_Corrupted, File_System_Unsupported, File_System_Unmounted 
  #6, File_Not_Found, Directory_Not_Found, File_Or_Directory_Not_Found
  #9, File_Already_Exist, Directroy_Already_Exist
  #11, Directory_Is_Full, Disk_Is_Full
  #13, Directory_Not_Empty, File_Is_Read_Only
  #15, Checksum_Error, Reboot_error

  #2_147_483_136, Max_File_Size, #65_535, Max_Directory_Size

OBJ

VAR

  long diskSignature, volumeIdentification, partitionStart, partitionSize, FATSectorSize, FATType 
  long freeClusterCount, nextFreeCluster, firstDataSector, countOfClusters, hiddenSectors, rootCluster
  long currentCluster, currentByte, currentSector, previousCluster, previousByte, previousSector
  long currentDirectory, currentFile, filePosition, fileSize 

  word currentTime, currentDate, reservedSectorCount, externalFlags
  word rootDirectorySectors, rootDirectorySectorNumber, fileSystemInfo, backupBootSector 

  byte partitionMountedFlag, errorNumberFlag, fileOpenCloseFlag, fileReadWriteFlag
  byte sectorsPerCluster, numberOfFATs, mediaType, reservedByWindowsNT, cleanShutdown, hardError 
  byte workingDirectoryPathIndex, workingDirectoryTooDeep   

  byte dataBlock[512], directoryEntry[32], directoryEntryName[12], workingDirectoryPath[66]
  byte volumeLabel[12], cardUniqueIDCopy[17]   
  byte unformatedNameBuffer[13], formatedNameBuffer[12] 

PUB readByte '' 28 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Reads a byte from the file that is currently open and advances the position by one.                                      │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Returns the next byte to read from the file. At the end of file returns the last byte in the file repeatedly.            │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  readData(@result, 1)

PUB readShort '' 28 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Reads a short from the file that is currently open and advances the position by two.                                     │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Returns the next short to read from the file. At the end of file returns the last byte in the file repeatedly.           │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  readData(@result, 2)

PUB readLong '' 28 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Reads a long from the file that is currently open and advances the position by four.                                     │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Returns the next long to read from the file. At the end of file returns the last byte in the file repeatedly.            │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  readData(@result, 4)

PUB writeByte(value) '' 29 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Writes a byte to the file that is currently open and advances the position by one.                                       │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Additionally this function will do nothing if the currently open file is not open for writing.                           │              
'' │                                                                                                                          │
'' │ The maximum file size is 2,147,483,136 bytes. Exceeding this causes an error.                                            │
'' │                                                                                                                          │
'' │ Value - A byte to write to the file.                                                                                     │                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  writeData(@value, 1)   
  
PUB writeShort(value) '' 29 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Writes a short to the file that is currently open and advances the position by two.                                      │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Additionally this function will do nothing if the currently open file is not open for writing.                           │              
'' │                                                                                                                          │
'' │ The maximum file size is 2,147,483,136 bytes. Exceeding this causes an error.                                            │
'' │                                                                                                                          │
'' │ Value - A short to write to the file.                                                                                    │                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  writeData(@value, 2)

PUB writeLong(value) '' 29 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Writes a long to the file that is currently open and advances the position by four.                                      │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Additionally this function will do nothing if the currently open file is not open for writing.                           │              
'' │                                                                                                                          │
'' │ The maximum file size is 2,147,483,136 bytes. Exceeding this causes an error.                                            │
'' │                                                                                                                          │
'' │ Value - A long to write to the file.                                                                                     │                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  writeData(@value, 4)

PUB writeString(stringPointer) '' 29 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Writes a string to the file that is currently open and advances the position by the string length.                       │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Additionally this function will do nothing if the currently open file is not open for writing.                           │              
'' │                                                                                                                          │
'' │ The maximum file size is 2,147,483,136 bytes. Exceeding this causes an error.                                            │
'' │                                                                                                                          │
'' │ StringPointer - A pointer to a string to write to the file.                                                              │                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
    
  writeData(stringPointer, strsize(stringPointer))

PUB readData(addressToPut, count) | index '' 25 Stack Longs 

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Reads data from the file that is currently open and advances the position by that amount of data.                        │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ AddressToPut - A pointer to the start of a data buffer to fill from disk.                                                │
'' │ Count - The amount of data to read from disk. The data buffer must be at least this large.                               │                                                                                        
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  count #>= 0
  repeat while((count > 0) and partitionMountedFlag and fileOpenCloseFlag)

    if((currentByte >> 9) <> (filePosition >> 9))
      flushData

    currentByte := filePosition   
    ifnot(readWriteCurrentCluster("R", "F"))
      quit
  
    index := (filePosition & $1FF)
    result := (count <# (512 - index))    
    bytemove(addressToPut, @dataBlock[index], result)

    count -= result
    addressToPut += result
    filePosition := ((filePosition + result) <# ((fileSize - 1) #> 0))
   
PUB writeData(addressToGet, count) | index '' 25 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Writes data to the file that is currently open and advances the position by that amount of data.                         │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Additionally this function will do nothing if the currently open file is not open for writing.                           │              
'' │                                                                                                                          │
'' │ The maximum file size is 2,147,483,136 bytes. Exceeding this causes an error.                                            │
'' │                                                                                                                          │
'' │ AddressToGet - A pointer to the start of a data buffer to write to disk.                                                 │
'' │ Count - The amount of data to write to disk. The data buffer must be at least this large.                                │                                                                               
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  count #>= 0
  repeat while((count > 0) and partitionMountedFlag and fileOpenCloseFlag and fileReadWriteFlag)

    if((currentByte >> 9) <> (filePosition >> 9))
      flushData

    currentByte := filePosition  
    ifnot(readWriteCurrentCluster("W", "F"))
      quit
    
    index := (filePosition & $1FF)
    result := (count <# (512 - index))
    bytemove(@dataBlock[index], addressToGet, result)

    count -= result
    addressToGet += result
    filePosition += result
    fileSize #>= filePosition 

PUB flushData '' 12 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Writes buffered data to disk. All file writes are buffered and are not written to disk immediantly.                      │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' │                                                                                                                          │
'' │ Additionally this function will do nothing if the currently open file is not open for writing.                           │ 
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘  

  if(partitionMountedFlag and fileOpenCloseFlag and fileReadWriteFlag)
    readWriteCurrentSector("W")  

PUB checkFilePosition '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the current byte position within a file for reading and writing.                                                 │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘ 

  return (filePosition & (partitionMountedFlag and fileOpenCloseFlag))
  
PUB changeFilePosition(position) | backupPosition '' 17 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Changes the current byte position within a file for reading and writing.                                                 │
'' │                                                                                                                          │
'' │ This function will do nothing if a file is not currently open or if a partition is not currently mounted.                │ 
'' │                                                                                                                          │
'' │ Position - A byte position in the file.                                                                                  │                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
  
  if(partitionMountedFlag and fileOpenCloseFlag)

    position := ((position <# (fileSize - 1)) #> 0)
    backupPosition := position
    position >>= 9
  
    if(position <> currentSector)
      flushData
    
      position /= sectorsPerCluster
      currentSector /= sectorsPerCluster
    
      if(position <> currentSector)
        if(position < currentSector)
          currentCluster := currentFile

        repeat until(position == result++)
                                                       
          readWriteFATBlock(currentCluster, "R")
          currentCluster := readFATEntry(currentCluster)

          if((currentCluster =< 1) or (FATEndOfClusterValue =< currentCluster))
            partitionMountedFlag := false
            errorNumberFlag := File_System_Corrupted
            abort @FSCorrupted

      result := true            

    filePosition := backupPosition
    currentByte := backupPosition

    if(result)
      readWriteCurrentSector("R")
  
PUB closeFile '' 15 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Closes the currently open file in the current directory.                                                                 │
'' │                                                                                                                          │
'' │ All files opened for writing or appending must be closed or they will become corrupted and must be fixed with scandisk.  │                                      
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │                                                                            
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  flushData
  
  if(partitionMountedFlag and fileOpenCloseFlag~)

    currentByte := previousByte
    currentSector := previousSector
    currentCluster := previousCluster


    readWriteCurrentSector("R")

    wordToBlock((currentByte + 18), readClock)

    if(fileReadWriteFlag)

      wordToBlock((currentByte + 22), currentTime)
      wordToBlock((currentByte + 24), currentDate)
      longToBlock((currentByte + 28), fileSize)
            
      dataBlock[(currentByte + 11) & $1FF] |= $20

    readWriteCurrentSector("W")

  listReset      
                               
PUB openFile(fileName, mode) '' 33 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Opens a file in the current directory for reading, writing, or appending.                                                │
'' │                                                                                                                          │
'' │ All files opened for writing or appending must be closed or they will become corrupted and must be fixed with scandisk.  │ 
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │
'' │                                                                                                                          │
'' │ The "." and ".." entries are ignored by this function.                                                                   │                                                                   
'' │                                                                                                                          │
'' │ Returns a pointer to the name of the opened file.                                                                        │ 
'' │                                                                                                                          │
'' │ FileName - The name of the file to open for reading, writing or appending.                                               │
'' │ Mode - A character specifing the mode to use. R-Read, W-Write, A-Append. By default the file is opended for reading.     │                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
 
  result := unformatName(listFind(formatName(fileName), @fileNotFound))

  if(listIsDirectory)
    errorNumberFlag := File_Not_Found
    abort @fileNotFound

  currentFile := listCluster
  ifnot(currentFile)
    currentFile := createClusterChain(0)

    readWriteCurrentSector("R")

    wordToBlock((currentByte + 18), readClock)
    wordToBlock((currentByte + 22), currentTime)
    wordToBlock((currentByte + 24), currentDate)
    dataBlock[(currentByte + 11) & $1FF] |= $20         
    
    wordToBlock((currentByte + 26), (currentFile & $FFFF))
    wordToBlock((currentByte + 20), (currentFile >> 16))
    
    readWriteCurrentSector("W")

  fileReadWriteFlag := (findByte(mode, "w", "W") | findByte(mode, "a", "A"))

  if(listIsReadOnly and fileReadWriteFlag)
    errorNumberFlag := File_Is_Read_Only
    abort @fileIsReadOnly

  filePosition := 0  
  fileSize := listSize
  
  previousByte := currentByte~
  previousSector := currentSector~
  previousCluster := currentCluster
  currentCluster := currentFile
  
  readWriteCurrentSector("R")
  fileOpenCloseFlag := true 
  
  if(findByte(mode, "a", "A"))
    changeFilePosition(fileSize)
    filePosition := fileSize

PUB newFile(fileName) '' 40 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Creates a new file in the current directory.                                                                             │
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │                                                                   
'' │                                                                                                                          │
'' │ Returns a pointer to the name of the file. List functions are not valid after calling this function.                     │
'' │                                                                                                                          │
'' │ FileName - The name of the new file to create. Must be a new unique name in the current directory.                       │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  return unformatName(listNew(formatName(fileName), $20, readClock, currentTime, 0, "F"))
  listReset
  
PUB newDirectory(directoryName) '' 40 Stack Longs  

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Creates a new directory in the current directory.                                                                        │
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │                                                                   
'' │                                                                                                                          │
'' │ Returns a pointer to the name of the directory. List functions are not valid after calling this function.                │
'' │                                                                                                                          │
'' │ DirectoryName - The name of the new directory to create. Must be a new unique name in the current directory.             │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  result := unformatName(listNew(formatName(directoryName), $30, readClock, currentTime, 0, "D"))

  directoryName := currentDirectory
  currentDirectory := currentFile
  
  listNew(@dot, $10, currentDate, currentTime, currentDirectory, "F") 
  listNew(@dotdot, $10, currentDate, currentTime, directoryName, "F")

  currentDirectory := directoryName
  listReset

PUB deleteEntry(entryName) '' 32 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Deletes a file or directory in the current directory. Cannot delete non empty directories.                               │                                                                                                     
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │
'' │                                                                                                                          │
'' │ This function will throw an error if it is called on a read only file or directory.                                      │
'' │                                                                                                                          │                                                                                                                      
'' │ The "." and ".." entries are ignored by this function.                                                                   │                                                                   
'' │                                                                                                                          │
'' │ Returns a pointer to the name of the deleted file or directory. List functions are not valid after calling this function.│
'' │                                                                                                                          │
'' │ EntryName - The name of the file or directory to delete.                                                                 │                                                                                                                                                                                                                                              
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
           
  result := unformatName(listFind(formatName(entryName), @fileOrDirectoryNotFound))

  if(listIsReadOnly)
    errorNumberFlag := File_Is_Read_Only
    abort @fileIsReadOnly 
  
  currentFile := listCluster
  if(listIsDirectory)
  
    previousByte := currentByte~
    previousSector := currentSector~
    previousCluster := currentCluster
    currentCluster := currentFile

    repeat 
      entryName := listDirectory("R")

      ifnot(entryName)
        quit
      
      ifnot(strcomp(entryName, @dot) or strcomp(entryName, @dotdot))
        errorNumberFlag := Directory_Not_Empty
        abort string("Directory Not Empty")

    currentByte := previousByte
    currentSector := previousSector
    currentCluster := previousCluster
    readWriteCurrentSector("R")

  byteToBlock(currentByte, $E5)
  readWriteCurrentSector("W")

  destroyClusterChain(currentFile)
  listReset    

PUB renameEntry(entryNameToChange, entryNameToChangeTo) '' 33 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Renames a file or directory in the current directory. The new name must be unique in the current directory.              │
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │
'' │                                                                                                                          │
'' │ This function will throw an error if it is called on a read only file or directory.                                      │
'' │                                                                                                                          │
'' │ The "." and ".." entries are ignored by this function.                                                                   │                                                                   
'' │                                                                                                                          │
'' │ Returns a pointer to the old name of the file or directory. List functions are not valid after calling this function.    │ 
'' │                                                                                                                          │
'' │ EntryNameToChange - The name of the file or directory to change.                                                         │ 
'' │ EntryNameToChangeTo - The name of the file or directory to change to.                                                    │                                                                         
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  listDuplicate(formatName(entryNameToChangeTo))  
  result := unformatName(listFind(formatName(entryNameToChange), @fileOrDirectoryNotFound))
  
  if(listIsReadOnly)
    errorNumberFlag := File_Is_Read_Only
    abort @fileIsReadOnly

  bytemove(@dataBlock[currentByte & $1FF], formatName(entryNameToChangeTo), 11)
  
  wordToBlock((currentByte + 18), readClock)
  wordToBlock((currentByte + 22), currentTime)
  wordToBlock((currentByte + 24), currentDate)
  dataBlock[(currentByte + 11) & $1FF] |= $20

  readWriteCurrentSector("W")
  listReset

PUB changeAttributes(entryName, newAttributes) '' 33 Stack Longs 

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Changes the attributes of a file or directory in the current directory.                                                  │                
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │
'' │                                                                                                                          │
'' │ The "." and ".." entries are ignored by this function.                                                                   │                                                                   
'' │                                                                                                                          │
'' │ Returns a pointer to the name of the file or directory. List functions are not valid after calling this function.        │
'' │                                                                                                                          │                                                                                                                 
'' │ EntryName - The name of the file or directory to change the attributes of.                                               │
'' │ NewAttributes - A string of characters containing the new set of attributes. A-Archive, S-System, H-Hidden, R-Read Only. │                                                         
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  result := unformatName(listFind(formatName(entryName), @fileOrDirectoryNotFound))

  wordToBlock((currentByte + 18), readClock)
  wordToBlock((currentByte + 22), currentTime)
  wordToBlock((currentByte + 24), currentDate)

  byteToBlock((currentByte + 11), (   (listIsDirectory & $10)                   {
                                  } | ($20 & findCharacter(newAttributes, "A")) {
                                  } | ($04 & findCharacter(newAttributes, "S")) {
                                  } | ($02 & findCharacter(newAttributes, "H")) {
                                  } | ($01 & findCharacter(newAttributes, "R")) ) )
                                  
  readWriteCurrentSector("W")
  listReset

PUB changeDirectory(directoryName) '' 32 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Searches the current directory for the specified directory and enters that directory.                                    │
'' │                                                                                                                          │
'' │ Use "\" to return the root directory. Directory names and or paths following the "\" are not evaluated.                  │
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │
'' │                                                                                                                          │
'' │ Returns a pointer to the working directory path string. List functions are not valid after calling this function.        │                                                                                                                                 
'' │                                                                                                                          │
'' │ DirectoryName - The name of the directory to search for in the current directory and enter into.                         │                                                                                                                                                                                                                                  
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘ 
  
  if(findByte(byte[directoryName], "/", "\"))
  
    bytemove(@workingDirectoryPath, string("\"), 2)
    workingDirectoryPathindex := 1
    workingDirectoryTooDeep := false
    currentDirectory := 0
    
  elseifnot((byte[directoryName] == ".") and (not(byte[directoryName + 1])))

    result := formatName(directoryName)
    if((byte[directoryName] == ".") and (byte[directoryName + 1] == ".") and (not(byte[directoryName + 2])))
      result := @dotDot      

    directoryName := unformatName(listFind(result, @directoryNotFound))
    
    ifnot(listIsDirectory)
      errorNumberFlag := Directory_Not_Found 
      abort @directoryNotFound

    ifnot(workingDirectoryTooDeep)
      
      if(result == @dotDot)
    
        repeat until(workingDirectoryPath[--workingDirectoryPathIndex - 1] == "\")
        workingDirectoryPath[workingDirectoryPathIndex] := 0

      else

        if(workingDirectoryPathIndex == 65)
          workingDirectoryTooDeep := true
      
        repeat strsize(directoryName)
          if((workingDirectoryPathIndex => 64) or (byte[directoryName] == " "))
            quit
        
          workingDirectoryPath[workingDirectoryPathIndex++] := byte[directoryName++]

        if(workingDirectoryPathIndex =< 64)  
          workingDirectoryPath[workingDirectoryPathIndex++] := "\"
          workingDirectoryPath[workingDirectoryPathIndex] := 0

    if(workingDirectoryTooDeep)
      bytemove(@workingDirectoryPath, string("\\..."), 6)         

    currentDirectory := listCluster

  listReset  
  return listWorkingDirectory

PUB listWorkingDirectory '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns a pointer to the working directory path string.                                                                  │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return @workingDirectoryPath
  
PUB listName(fromTheTop) '' 27 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns a pointer to the name of the next file or directory string in the current directory.                             │
'' │                                                                                                                          │
'' │ After the last file or directory in the current directory is listed "listName" returns 0.                                │
'' │                                                                                                                          │
'' │ After returning zero the next call to "listName" will start from the top of the current directory.                       │   
'' │                                                                                                                          │
'' │ If the partition is not mounted or an error occurs this function will abort and return a string describing that error.   │
'' │                                                                                                                          │
'' │ FromTheTop - If "T" "listName returns 0 and next call to "listName" will start from the top of the current directory.    │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  ifnot(findByte(fromTheTop, "t", "T"))
    return unformatName(listDirectory("R"))
  
  closeFile
    
PUB listSize '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the size of current file or directory pointed to by "listName". Directories have no size.                           │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that files information.                                          │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the size of the file or directory in bytes. Maximum file size is 2,147,483,136 bytes.                            │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return ( ( (directoryEntry[28]          {
           } | (directoryEntry[29] << 08)   {
           } | (directoryEntry[30] << 16)   {
           } | (directoryEntry[31] << 24) ) {
           } <# $7FFFFE00) #> 0)

PUB listCreationDay '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the creation day of the current file or directory pointed to by "listName".                                         │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the creation day of the file or directory.                                                                       │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (directoryEntry[16] & $1F)

PUB listCreationMonth '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the creation month of the current file or directory pointed to by "listName".                                       │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the creation month of the file or directory.                                                                     │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (((directoryEntry[17] & $1) << 3) | (directoryEntry[16] >> 5))

PUB listCreationYear '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the creation year of the current file or directory pointed to by "listName".                                        │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the creation year of the file or directory.                                                                      │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return ((directoryEntry[17] >> 1) + 1980)

PUB listCreationCentiseconds '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the creation centisecond of the current file or directory pointed to by "listName".                                 │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the creation centisecond of the file or directory.                                                               │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (directoryEntry[13] // 100)   
    
PUB listCreationSeconds '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the creation second of the current file or directory pointed to by "listName".                                      │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the creation second of the file or directory.                                                                    │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (((directoryEntry[14] & $1F) << 1) +  (directoryEntry[13] / 100))

PUB listCreationMinutes '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the creation minute of the current file or directory pointed to by "listName".                                      │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the creation minute of the file or directory.                                                                    │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (((directoryEntry[15] & $7) << 3) | (directoryEntry[14] >> 5))

PUB listCreationHours '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the creation hour of the current file or directory pointed to by "listName".                                        │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the creation hour of the file or directory.                                                                      │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (directoryEntry[15] >> 3)

PUB listAccessDay '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last day of access of the current file or directory pointed to by "listName".                                   │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the last acess day of the file or directory.                                                                     │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (directoryEntry[18] & $1F)

PUB listAccessMonth '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last month of access of the current file or directory pointed to by "listName".                                 │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the last acess month of the file or directory.                                                                   │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (((directoryEntry[19] & $1) << 3) | (directoryEntry[18] >> 5))

PUB listAccessYear '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last year of access of the current file or directory pointed to by "listName".                                  │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the last acess year of the file or directory.                                                                    │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return ((directoryEntry[19] >> 1) + 1980)

PUB listModificationDay '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last day of modification of the current file or directory pointed to by "listName".                             │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the modification day of the file or directory.                                                                   │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (directoryEntry[24] & $1F) 

PUB listModificationMonth '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last month of modification of the current file or directory pointed to by "listName".                           │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the modification month of the file or directory.                                                                 │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (((directoryEntry[25] & $1) << 3) | (directoryEntry[24] >> 5))

PUB listModificationYear '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last year of modification of the current file or directory pointed to by "listName".                            │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the modification year of the file or directory.                                                                  │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return ((directoryEntry[25] >> 1) + 1980)

PUB listModificationSeconds '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last second of modification of the current file or directory pointed to by "listName".                          │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the modification second of the file or directory.                                                                │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return ((directoryEntry[22] & $1F) << 1) 

PUB listModificationMinutes '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last minute of modification of the current file or directory pointed to by "listName".                          │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the modification minute of the file or directory.                                                                │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (((directoryEntry[23] & $7) << 3) | (directoryEntry[22] >> 5))

PUB listModificationHours '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Gets the last hour of modification of the current file or directory pointed to by "listName".                            │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns the modification hour of the file or directory.                                                                  │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (directoryEntry[23] >> 3)  

PUB listIsReadOnly '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns whether or not the current file or directory pointed to by "listName" is read only.                              │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns true or false.                                                                                                   │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    result or= (directoryEntry[11] & $1)

PUB listIsHidden '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns whether or not the current file or directory pointed to by "listName" is hidden.                                 │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns true or false.                                                                                                   │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    result or= (directoryEntry[11] & $2)

PUB listIsSystem '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns whether or not the current file or directory pointed to by "listName" is a system file.                          │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns true or false.                                                                                                   │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    result or= (directoryEntry[11] & $4)  
  
PUB listIsDirectory '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns whether or not the current file or directory pointed to by "listName" is a directory.                            │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns true or false.                                                                                                   │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    result or= (directoryEntry[11] & $10)

PUB listIsArchive '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns whether or not the current file or directory pointed to by "listName" has been modified since the last backup.   │
'' │                                                                                                                          │
'' │ If a file is currently open this function will retrieve that file's information.                                         │
'' │                                                                                                                          │
'' │ If "listName" did not succed or was not previously called the value returned is invalid.                                 │
'' │                                                                                                                          │
'' │ Returns true or false.                                                                                                   │                                                                                                                                                                                                      
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘  

  if(partitionMountedFlag) 
    result or= (directoryEntry[11] & $20)
   
PUB checkCountOfClusters '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the count of clusters for the current partition.                                                                 │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (countOfClusters - 1)

PUB checkTotalSectors '' 3 Stack longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the count of sectors for the current partition.                                                                  │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)       
    return (partitionSize - firstDataSector)  
  
PUB checkSectorsPerCluster '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the count of sectors per cluster for the current partition.                                                      │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)    
    return sectorsPerCluster

PUB checkBytesPerSector '' 3 Stack Longs 

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the count of bytes per sector for the current partition.                                                         │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return 512
    
PUB checkUsedSectorCount(mode) '' 18 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the current used sector count on this partition. Will do nothing if the card is not mounted.                     │                  
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │                                                                                       
'' │ In fast mode this function will return the last valid used sector count if avialable. This is an estimate value.         │
'' │                                                                                                                          │
'' │ In slow mode this function will compute the used sector count by scanning the entire FAT. This can take a long time.     │
'' │                                                                                                                          │
'' │ One sector is equal to 512 bytes. Multiply the used sector count by 512 to determine the number of used bytes.           │
'' │                                                                                                                          │
'' │ This function also finds the next free cluster needed for creating and extending files and directories.                  │
'' │                                                                                                                          │
'' │ Call this function when running out of disk space to find the next free cluster if available.                            │
'' │                                                                                                                          │
'' │ This function can be called while a file is open to find more disk space. The file will still be open afterwards.        │
'' │                                                                                                                          │
'' │ If the last valid used sector count is not avialable when using fast mode this function will enter slow mode,            │                                                                                                                   
'' │                                                                                                                          │                                                                                                                
'' │ Mode - A character specifing the mode to use. F-Fast, S-Slow. Default slow.                                              │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return (((countOfClusters - 1) * sectorsPerCluster) - checkFreeSectorCount(mode)) 
  
PUB checkFreeSectorCount(mode) '' 14 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the current free sector count on this partition. Will do nothing if the card is not mounted.                     │                  
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │                                                                                       
'' │ In fast mode this function will return the last valid free sector count if avialable. This is an estimate value.         │
'' │                                                                                                                          │
'' │ In slow mode this function will compute the free sector count by scanning the entire FAT. This can take a long time.     │
'' │                                                                                                                          │
'' │ One sector is equal to 512 bytes. Multiply the free sector count by 512 to determine the number of free bytes.           │
'' │                                                                                                                          │
'' │ This function also finds the next free cluster needed for creating and extending files and directories.                  │
'' │                                                                                                                          │
'' │ Call this function when running out of disk space to find the next free cluster if available.                            │
'' │                                                                                                                          │
'' │ This function can be called while a file is open to find more disk space. The file will still be open afterwards.        │ 
'' │                                                                                                                          │
'' │ If the last valid free sector count is not avialable when using fast mode this function will enter slow mode,            │                                                                                                                   
'' │                                                                                                                          │                                                                                                                
'' │ Mode - A character specifing the mode to use. F-Fast, S-Slow. Default slow.                                              │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    flushData
  
    if(findByte(mode, "f", "F") and (freeClusterCount <> $FFFFFFFF))
      result := freeClusterCount
 
    else
      repeat mode from 0 to countOfClusters

        ifnot(FATEntryNumber(mode))
          readWriteFATBlock(mode, "R")
                                              
        result -= (not(readFATEntry(mode)))

        ifnot(result)
          nextFreeCluster := ((mode + 1) <# countOfClusters)

      freeClusterCount := result
      readWriteCurrentSector("R")
    
    result *= sectorsPerCluster

PUB checkVolumeLabel '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns a pointer to the volume label string.                                                                            │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return @volumeLabel

PUB checkFileSystemType '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns a pointer to the file system type string.                                                                        │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    if(FATType)
      return string("FAT32   ")
    return string("FAT16   ")    

PUB checkVolumeIdentification '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the volume identification number.                                                                                │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return volumeIdentification  

PUB checkDiskSignature '' 3 Stack Longs 

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns the disk signature number.                                                                                       │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  if(partitionMountedFlag)
    return diskSignature

PUB checkErrorNumber '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns true if the file system incurred an error and false if not. The true number returned is the error number.        │                                                                                                                                                                                                     
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  return errorNumberFlag~

PUB checkPartitionMounted '' 3 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Returns true if the file system is still mounted and false if not.                                                       │                                                                                                                                 
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  result or= partitionMountedFlag

PUB bootPartition(fileName) | bootSectors[64] '' 101 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Reboots the propeller chip to run the selected valid spin BIN or EEPROM file.                                            │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ FileName - The name of the file to reboot from in the current directory.                                                 │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
   
  longfill(@bootSectors, 0, 64)
  openFile(fileName~, "R")
            
  repeat (listSize <# 32768)
    result += readByte

    ifnot($1FF & fileName++)
      bootSectors[fileName >> 9] := (partitionStart + FATFirstSectorInCluster(currentCluster) + FATWorkingSectorInCluster)  

  result &= $FF
  changeFilePosition(6)
  fileName := readShort
  closeFile
  
  if((result and (result <> $14)) or (fileName <> $10))
    errorNumberFlag := Checksum_Error  
    abort string("Checksum Error")

  unmountPartition
  readWriteBlock(@bootSectors, "B")
  
  partitionMountedFlag := false
  errorNumberFlag := Reboot_Error
  abort string("Reboot Error") 

PUB formatPartition(partition, newVolumeLabel) '' 33 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Formats and mounts the specified partition. The file system may be a FAT16/32 file system with up to ~1TB of space.      │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ Returns a pointer to the volume label.                                                                                   │
'' │                                                                                                                          │
'' │ Parition - Partition number to mount (between 0 and 3). The default partition number is 0.                               │
'' │ VolumeLabel - New volume label for the partition.                                                                        │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  mountPartition(partition, "C")
  listReset
  
  repeat while(readWriteCurrentCluster("R", "D"))
    bytefill(@dataBlock, 0, 512)
    readWriteCurrentSector("W")
    currentByte += 512

  listNew(formatName(newVolumeLabel), $8, readClock, currentTime, 0, "F")
  
  bytefill(@dataBlock, 0, 512)  
  repeat result from reservedSectorCount to (rootDirectorySectorNumber - 1)
    readWriteBlock(result, "W")
   
  readWriteFATBlock(0, "R")
  writeFATEntry(0, ($0FFFFF00 | mediaType))
  writeFATEntry(1, $0FFFFFFF)
  readWriteFATBlock(0, "W")
  
  if(FATType)
    readWriteFATBlock(rootCluster, "R")
    writeFATEntry(rootCluster, true)
    readWriteFATBlock(rootCluster, "W") 

  listReset
  return unformatName(listDirectory("V"))  

PUB mountPartition(partition, checkDisk) '' 28 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Mounts the specified partition. The file system may be a FAT16/32 file system with up to ~1TB of space.                  │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │
'' │                                                                                                                          │
'' │ File sizes up to 2,147,483,136 bytes are supported.                                                                      │
'' │                                                                                                                          │
'' │ Directory sizes up to 65,535 entries are supported.                                                                      │
'' │                                                                                                                          │
'' │ Additionally the check disk flag can be raised to call scan disk on an improperly unmounted volume.                      │
'' │                                                                                                                          │
'' │ Returns a pointer to the volume label.                                                                                   │
'' │                                                                                                                          │
'' │ Parition - Partition number to mount (between 0 and 3). The default partition number is 0.                               │
'' │ CheckDisk - Raises the check disk flag. C-Raise Flag                                                                     │                                                                                                                                                                        
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  unmountPartition
  
  partitionStart := 0
  readWriteBlock(0, "M")
  
  bytemove(@cardUniqueIDCopy, @cardUniqueID, 17)
  readWriteBlock(0, "R")

  if(blockToWord(510) <> $AA55)
    errorNumberFlag := File_System_Corrupted
    abort @FSCorrupted

  diskSignature := blockToLong(440)
    
  partition := (((partition <# 3) #> 0) << 4)
  if((blockToByte(0) <> $EB) and (blockToByte(0) <> $E9))
  
    case(blockToByte(450 + partition) & $F)      
      $0 .. $3, $5, $7 .. $A, $D, $F:
        errorNumberFlag := File_System_Unsupported 
        abort @FSUnsupported

    partitionStart := blockToLong(454 + partition)
    readWriteBlock(0, "R")  
    
  if(blockToWord(510) <> $AA55)
    errorNumberFlag := File_System_Corrupted
    abort @FSCorrupted    

  if(blockToWord(11) <> 512)
    errorNumberFlag := File_System_Unsupported
    abort @FSUnsupported

  sectorsPerCluster := blockToByte(13)
  reservedSectorCount := blockToWord(14)  
  numberOfFATs := blockToByte(16)
  externalFlags := 0

  mediaType := blockToByte(21)
  hiddenSectors := blockToLong(28)
  
  partitionSize := blockToWord(19)
  ifnot(partitionSize)
    partitionSize := blockToLong(32)

  FATSectorSize := blockToWord(22)
  ifnot(FATSectorSize) 
    FATSectorSize := blockToLong(36)

  rootDirectorySectors := (blockToWord(17) >> 4)
  rootDirectorySectorNumber := (reservedSectorCount + (numberOfFATs * FATSectorSize))

  firstDataSector := (rootDirectorySectorNumber + rootDirectorySectors)
  countOfClusters := ((partitionSize - firstDataSector) / sectorsPerCluster) 

  nextFreeCluster := 2
  freeClusterCount := $FFFFFFFF

  if(countOfClusters < 4085)
    errorNumberFlag := File_System_Unsupported 
    abort @FSUnsupported
  
  FATType := false
  if(65525 =< countOfClusters++) 
    FATType := true

  result := (28 & FATType)
  reservedByWindowsNT := blockToByte(37 + result) 
  volumeIdentification := blockToLong(39 + result)

  if(findByte(checkDisk, "c", "C"))
    dataBlock[37 + partition] |= $3                  
    readWriteBlock(0, "W")   

  if(FATType)
  
    if(blockToWord(40) & $80)
      numberOfFATs := 1
      externalFlags := (blockToWord(40) & $F)

    if(blockToWord(42))
      errorNumberFlag := File_System_Unsupported  
      abort @FSUnsupported

    rootCluster := blockToLong(44)
    fileSystemInfo := blockToWord(48) 
    backupBootSector := blockToWord(50)

    readWriteBlock(fileSystemInfo, "R")
    
    if(blockToWord(510) <> $AA55)
      errorNumberFlag := File_System_Corrupted
      abort @FSCorrupted

    freeClusterCount := blockToLong(488)
    nextFreeCluster := blockToLong(492)

    if(nextFreeCluster == $FFFFFFFF)
      nextFreeCluster := 2

  readWriteFATBlock(0, "R")
  result := ($8000 << (12 & FATType))
  cleanShutdown := (readFATEntry(1) & result)
  hardError := (readFATEntry(1) & result)

  bytemove(@workingDirectoryPath, string("\"), 2)
  workingDirectoryPathIndex := 1
  workingDirectoryTooDeep := false
  currentDirectory := 0
     
  partitionMountedFlag := true
  listReset
                        
  result := listDirectory("V")
  bytemove(@volumeLabel, result, 12) 
  
PUB unmountPartition '' 18 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Unmounts the specified partition.                                                                                        │
'' │                                                                                                                          │
'' │ If an error occurs this function will abort and return a pointer to a string describing that error.                      │                                                                                                                                                                                   
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  closeFile
  errorNumberFlag := 0
   
  if(partitionMountedFlag~)

    readWriteBlock(0, "R")
    dataBlock[37 + (28 & FATType)] &= $FC
    readWriteBlock(0, "W")    
  
    if(FATType)  
      readWriteBlock(fileSystemInfo, "R")
      longToBlock(freeClusterCount, 488)
      longToBlock(nextFreeCluster, 492)
      readWriteBlock(fileSystemInfo, "W")

PUB FATEngineStart(DOPin, CLKPin, DIPin, CSPin) '' 17 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Starts up the SDC driver running on a cog and checks out a lock for the driver.                                          │ 
'' │                                                                                                                          │
'' │ Returns true on success and false on failure.                                                                            │
'' │                                                                                                                          │
'' │ DOPin - The SPI data out pin from the SD2.0 card. Between 0 and 31.                                                      │
'' │ CLKPin - The SPI clock pin from the SD2.0 card. Between 0 and 31.                                                        │
'' │ DIPin - The SPI data in pin from the SD2.0 card. Between 0 and 31.                                                       │
'' │ CSPin - The SPI chip select pin from the SD2.0 card. Between 0 and 31.                                                   │
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  FATEngineStop

  readTimeout := (clkfreq / constant(8 * 8 * 10))  
  writeTimeout := (clkfreq / constant(8 * 8 * 4))

  DOPin := ((DOPin <# 31) #> 0)
  CLKPin := ((CLKPin <# 31) #> 0)
  DIPin := ((DIPin <# 31) #> 0)

  dataOutCounterSetup := (constant(%11000 << 26) + (CLKPin << 9) + DOPin)
  clockCounterSetup := (constant(%00100 << 26) + CLKPin)
  dataInCounterSetup := (constant(%00100 << 26) + DIPin)
  
  dataOutPin := (|<DOPin)
  clockPin := (|<CLKPin)   
  dataInPin := (|<DIPin)   
  chipSelectPin := (|<((CSPin <# 31) #> 0))   

  CBAAddress := @cardBlockAddress                                                          
  CSAAddress := @cardSectorAddress           
  CSCAddress := @cardSectorCount         
  CCFAddress := @cardCommandFlag                                                            
  CEFAddress := @cardErrorFlag     
  CUIAddress := @cardUniqueID   

  if((dataOutPin <> clockPin) and (dataOutPin <> dataInPin) and (dataOutPin <> chipSelectPin) and {
    }(clockPin <> dataInPin) and (clockPin <> chipSelectPin) and (dataInPin <> chipSelectPin)) 
    cardLockID := locknew
    if(++cardLockID)
  
      cardCogID := cognew(@initialization, 0)
      if(++cardCogID)
        return true

      FATEngineStop  

PUB FATEngineStop '' 6 Stack Longs

'' ┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
'' │ Shuts down the SDC driver running on a cog and returns the lock used by the driver.                                      │               
'' └──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

  bytemove(@cardUniqueID, 0, 17)
  
  if(cardCogID)
    cogstop(-1 + cardCogID~)

  if(cardLockID)
    lockret(-1 + cardLockID~)
  
PRI listDuplicate(entryName) ' 27 Stack Longs

  closeFile

  repeat

    result := listDirectory("R")
    
    if(strcomp(result, entryName))
    
      if(listIsDirectory)
        errorNumberFlag := Directroy_Already_Exist
        abort string("Directory Already Exist")

      errorNumberFlag := File_Already_Exist  
      abort string("File Already Exist")  

  while(result)

PRI listFind(entryName, errorMessage) ' 28 Stack Longs

  closeFile
  
  repeat

    result := listDirectory("R")

    ifnot(result)
      errorNumberFlag := File_Or_Directory_Not_Found
      abort errorMessage
    
  until(strcomp(entryName, result)) 

  currentByte -= 32

PRI listNew(entryName, entryAttributes, entryDate, entryTime, entryCluster, entryType) ' 36 Stack Longs

  listDuplicate(entryName)
  
  repeat while(readWriteCurrentCluster("W", "D"))
    
    if((blockToByte(currentByte) <> $E5) and blockToByte(currentByte))
      currentByte += 32 

    else

      if(entryType == "D")
        entryCluster := createClusterChain(0)
        currentFile := entryCluster
        readWriteCurrentSector("R")
 
      bytefill(@dataBlock[currentByte & $1FF], 0, 32) 
      bytemove(@dataBlock[currentByte & $1FF], entryName, 11)

      byteToBlock((currentByte + 11), entryAttributes)

      wordToBlock((currentByte + 14), entryTime)
      wordToBlock((currentByte + 16), entryDate)      
      wordToBlock((currentByte + 18), entryDate)
      wordToBlock((currentByte + 22), entryTime)
      wordToBlock((currentByte + 24), entryDate)      

      wordToBlock((currentByte + 26), (entryCluster & $FFFF))
      wordToBlock((currentByte + 20), (entryCluster >> 16))
      
      readWriteCurrentSector("W") 
      return entryName

  errorNumberFlag := Directory_Is_Full    
  abort string("Directory Is Full")

PRI listDirectory(volumeIDAttribute) ' 23 Stack Longs

  if(fileOpenCloseFlag)
    closeFile

  repeat while(readWriteCurrentCluster("R", "D") and blockToByte(currentByte))
  
    if((blockToByte(currentByte) == $E5) or (((volumeIDAttribute == "V") ^ blockToByte(currentByte + 11)) & $8))
      currentByte += 32

    else
     
      bytemove(@directoryEntry, @dataBlock[currentByte & $1FF], 32)
      bytemove(@directoryEntryName, @directoryEntry, 11)
    
      if(directoryEntryName == $5)
        directoryEntryName := $E5
    
      currentByte += 32
      return @directoryEntryName

  listReset            

PRI listCluster ' 3 Stack Longs

  result := (directoryEntry[26] + (directoryEntry[27] << 8) + (directoryEntry[20] << 16) + (directoryEntry[21] << 24))
  
  if((result == 1) or (FATEndOfClusterValue =< result))
    partitionMountedFlag := false
    errorNumberFlag := File_System_Corrupted
    abort @FSCorrupted
  
PRI listReset '' 3 Stack Longs
 
  currentByte := 0
  currentSector := 0
  currentCluster := currentDirectory  

  bytefill(@directoryEntry, 0, 32)
  bytefill(@directoryEntryName, 0, 12)
  
PRI readWriteCurrentCluster(readWrite, fileOrDirectory) ' 19 Stack Longs

  ifnot(partitionMountedFlag)
    errorNumberFlag := File_System_Unmounted
    abort string("File System Unmounted") 

  ifnot(currentByte & $1FF)
                            
    if(fileOpenCloseFlag and (currentByte => $7FFFFE00))
      return false
      
    ifnot(fileOpenCloseFlag or (currentByte < constant(65536 * 32)))
      return false

    ifnot(currentcluster or FATType or ((currentByte >> 9) < rootDirectorySectors))
      return false

    if(((currentByte >> 9) <> currentSector) and (not(FATWorkingSectorInCluster)) and (FATType or currentcluster))
    
      result := currentCluster
      ifnot(result)
        result := rootCluster
        
      readWriteFATBlock(result, "R")  
      fileOrDirectory := readFATEntry(result)

      if(fileOrDirectory =< 1)
        partitionMountedFlag := false
        errorNumberFlag := File_System_Corrupted     
        abort @FSCorrupted
      
      if(fileOrDirectory => FATEndOfClusterValue)

        if(readWrite == "R")
          return false

        fileOrDirectory := createClusterChain(result)
      currentCluster := fileOrDirectory
      
    readWriteCurrentSector("R")
  return true
  
PRI readWriteCurrentSector(readWrite) ' 9 Stack Longs
  
  result := FATFirstSectorInCluster(currentCluster) + FATWorkingSectorInCluster 
  
  ifnot(currentCluster)
    result := rootDirectorySectorNumber + (currentByte >> 9)

    if(FATType)
      result := FATFirstSectorInCluster(rootCluster) + FATWorkingSectorInCluster

  readWriteBlock(result, readWrite)

  if(readWrite == "R")
    currentSector := (currentByte >> 9)
  
PRI findByte(byteToCompare, thisByte, thatByte) ' 6 Stack Longs

  if((byteToCompare == thisByte) or (byteToCompare == thatByte))
    return true
      
PRI findCharacter(charactersToSearch, characterToFind) | convertedCharacter ' 6 Stack Longs
    
  repeat strsize(charactersToSearch)

    convertedCharacter := byte[charactersToSearch++]
    case convertedCharacter
      "a" .. "z": convertedCharacter -= 32  

    if(convertedCharacter == characterToFind)
      return true

PRI unformatName(name) ' 4 Stack Longs

  if(name)    

    unformatedNameBuffer[12] := 0
  
    bytefill(@unformatedNameBuffer, " ", 12)
    bytemove(@unformatedNameBuffer, name, 8)

    repeat while(unformatedNameBuffer[++result] <> " ")
    unformatedNameBuffer[result++] := "."

    bytemove(@unformatedNameBuffer[result], @byte[name][8], 3)

    if(unformatedNameBuffer[result] == " ")
      unformatedNameBuffer[--result] := " "
  
    return @unformatedNameBuffer 

PRI formatName(name) ' 4 Stack Longs

  formatedNameBuffer[11] := 0

  bytefill(@formatedNameBuffer, " ", 11)

  repeat strsize(name--)

    if(byte[++name] == ".")
      result := 0

      repeat strsize(++name)
      
        if((result < 3) and (byte[name] > 31))
          formatedNameBuffer[8 + result++] := byte[name++]

      quit    

    if((result < 8) and (byte[name] > 31))
      formatedNameBuffer[result++] := byte[name]
      
  repeat result from 0 to 10

    case formatedNameBuffer[result]
      "a" .. "z": formatedNameBuffer[result] -= 32                            
      $22, "*" .. ",", "." .. "/", ":" .. "?", "[" .. "]", "|", $7F: formatedNameBuffer[result] := "_"

  if(formatedNameBuffer == " ")
    formatedNameBuffer := "_"            

  if(formatedNameBuffer == $E5)
    formatedNameBuffer := $5
  
  return @formatedNameBuffer

PRI createClusterChain(clusterToLink) ' 14 Stack Longs 

  readWriteFATBlock(nextFreeCluster, "R")
  repeat result from nextFreeCluster to countOfClusters
  
    ifnot(FATEntryNumber(result))
      readWriteFATBlock(result, "R")

    ifnot(readFATEntry(result))

      writeFATEntry(result, true)
      readWriteFATBlock(result, "W")

      nextFreeCluster := ((result + 1) <# countOfClusters)
      
      if(clusterToLink)
           
        readWriteFATBlock(clusterToLink, "R")
        writeFATEntry(clusterToLink, result)
        readWriteFATBlock(clusterToLink, "W")
      
      bytefill(@dataBlock, 0, 512)

      repeat clusterToLink from 0 to (sectorsPerCluster - 1)
        readWriteBlock((FATFirstSectorInCluster(result) + clusterToLink), "W")

      quit

    if(result => countOfClusters)
      nextFreeCluster := 2
      errorNumberFlag := Disk_Is_Full
      abort string("Disk Is Full")  

PRI destroyClusterChain(clusterToDestroy) ' 14 Stack Longs

  repeat while((1 < clusterToDestroy) and (clusterToDestroy < FATEndOfClusterValue))

    ifnot(result and (FATBlockNumber(result) == FATBlockNumber(clusterToDestroy)))
      readWriteFATBlock(clusterToDestroy, "R")

    result := clusterToDestroy
    clusterToDestroy := readFATEntry(clusterToDestroy)
    writeFATEntry(result, false)

    if(FATBlockNumber(result) <> FATBlockNumber(clusterToDestroy))      
      readWriteFATBlock(result, "W")

  if(result)
    readWriteFATBlock(result, "W")               

PRI readFATEntry(cluster) ' 8 Stack Longs

  cluster := FATEntryNumber(cluster)

  ifnot(FATType)
    return blockToWord(cluster)
  
  return (blockTolong(cluster) & $0FFFFFFF)  
      
PRI writeFATEntry(cluster, value) ' 10 Stack Longs

  cluster := FATEntryNumber(cluster)

  ifnot(FATType)
    wordToBlock(cluster, value)
  else
    longToBlock(cluster, ((value & $0FFFFFFF) | (blockTolong(cluster) & $F0000000)))  

PRI readWriteFATBlock(cluster, readWrite) ' 10 Stack Longs

  cluster := FATBlockNumber(cluster)
  result := externalFlags 
                                   
  repeat ((numberOfFATs & (readWrite == "W")) | (-(readWrite == "R")))
    readWriteBlock((reservedSectorCount + cluster + (FATSectorSize * result++)), readWrite)

PRI FATBlockNumber(cluster) ' 4 Stack Longs

  return (cluster >> (8 + FATType))

PRI FATEntryNumber(cluster) ' 4 Stack Longs

  return ((cluster & ($FF >> (-FATType))) << (1 - FATType))
 
PRI FATEndOfClusterValue ' 3 Stack Longs

  return ($FFF0 | (FATType & $0FFFFFF0))

PRI FATWorkingSectorInCluster ' 3 Stack Longs

  return ((currentByte >> 9) // sectorsPerCluster)

PRI FATFirstSectorInCluster(cluster) ' 4 Stack Longs

  return (((cluster - 2) * sectorsPerCluster) + firstDataSector)
  
PRI blockToLong(index) ' 4 Stack Longs

  bytemove(@result, @dataBlock[(index & $1FF) <# 508], 4)
  
PRI blockToWord(index) ' 4 Stack Longs 

  bytemove(@result, @dataBlock[(index & $1FF) <# 510], 2)
      
PRI blockToByte(index) ' 4 Stack Longs

  return dataBlock[index & $1FF]
                 
PRI longToBlock(index, value) ' 5 Stack Longs

  bytemove(@dataBlock[(index & $1FF) <# 508], @value, 4)
                               
PRI wordToBlock(index, value) ' 5 Stack Longs 

  bytemove(@dataBlock[(index & $1FF) <# 510], @value, 2)

PRI byteToBlock(index, value) ' 5 Stack Longs 

  dataBlock[index & $1FF] := value  

PRI readClock ' 3 + 11 Stack Longs 

{{
  ifnot(rtc.checkTime)
    errorNumberFlag := Clock_IO_Error 
    abort string("Clock I/O Error")
  
  currentTime := ((rtc.checkSecond >> 1) | (rtc.checkMinute << 5) | (rtc.checkHour << 11))
  currentDate := (rtc.checkDate | (rtc.checkMonth << 5) | ((rtc.checkYear - 1980) << 9))
}}
    
  return currentDate
  
PRI readWriteBlock(address, command) ' 5 Stack Longs

  if(strcomp(@cardUniqueID, @cardUniqueIDCopy) or (command == "M") or (command == "T"))
    repeat while(lockset(cardLockID - 1))

    cardSectorAddress := ((address + (partitionStart & (command <> "B"))) & (command <> "T"))
    cardBlockAddress := (@dataBlock & (command <> "B") & (command <> "T"))
    cardCommandFlag := command                  
    repeat while(cardCommandFlag)
      
    command := cardErrorFlag~
    lockclr(cardLockID - 1)

  if(command)
    partitionMountedFlag := false
    errorNumberFlag := Disk_IO_Error
    abort string("Disk I/O Error")    

DAT

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       SDC Driver
' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

                        org     0
                        
' //////////////////////Initialization///////////////////////////////////////////////////////////////////////////////////////// 
                        
initialization          mov     ctra,                 clockCounterSetup            ' Setup counter.

                        or      outa,                 chipSelectPin                ' Setup I/O Pins.
                        or      outa,                 dataInPin                    '
                        or      dira,                 chipSelectPin                '
                        or      dira,                 dataInPin                    '
                        or      dira,                 clockPin                     '

                        mov     cardCounter,          fiveHundredAndTwelve         ' Skip to instruction handle.
                        jmp     #instructionWait                                   ' 

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Command Center
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

instructionRetry        cmp     cardBuffer,           #"M" wc, wz                  ' Try at most 8 more times to mount card.
if_c                    cmp     cardBuffer,           #"F" wc, wz                  '
if_nc_or_z              djnz    cardBuffer,           #mountCard                   '

                        cmp     cardBuffer,           #"R" wz                      ' Try at most twice to read the block.
if_z                    djnz    cardBuffer,           #readBlock                   '

                        cmp     cardBuffer,           #"W" wz                      ' Try at most twice to write the block.                         
if_z                    djnz    cardBuffer,           #writeBlock                  '

                        cmp     cardBuffer,           #"B" wz                      ' Reboot the chip if booting failure.
if_z                    mov     buffer,               #$80                         ' 
if_z                    clkset  buffer                                             '

instructionError        wrbyte  maxPositiveInteger,   CEFAddress                   ' Assert error flag and unmount card.
                        mov     cardMounted,          #0                           '  

' //////////////////////Instruction Handle/////////////////////////////////////////////////////////////////////////////////////

instructionLoop         wrbyte  fiveHundredAndTwelve, CCFAddress                   ' Finish up old command.

instructionWait         rdbyte  cardBuffer,           CCFAddress                   ' Wait for a command to come.  
                        test    cardMounted,          maxPositiveInteger wc        ' 

                        cmp     cardBuffer,           #"B" wz                      ' If rebooting was requested do it.
if_z_and_nc             jmp     #instructionError                                  '                        
if_z_and_c              jmp     #rebootChip                                        '

                        cmp     cardBuffer,           #"R" wz                      ' If read block was requested do it.
if_z_and_nc             jmp     #instructionError                                  '
if_z_and_c              jmp     #readBlock                                         '

                        cmp     cardBuffer,           #"W" wz                      ' If write block was requested do it.
if_z_and_nc             jmp     #instructionError                                  '
if_z_and_c              jmp     #writeBlock                                        '

                        djnz    cardCounter,          #instructionSkip             ' Poll the card every so often.
                        mov     cardCounter,          fiveHundredAndTwelve         '
if_nc                   jmp     #instructionSkip                                   ' 
                        call    #cardStatus                                        '

instructionSkip         cmp     cardBuffer,           #"T" wz                      ' If tristating was requested do it.
if_z                    xor     dira,                 chipSelectPin                '
if_z                    xor     dira,                 dataInPin                    '
if_z                    xor     dira,                 clockPin                     '
if_z                    test    cardMounted,          #$1 wc                       '
if_z_and_nc             test    cardMounted,          #$2 wc                       '
if_z_and_c              xor     cardMounted,          #$1                          '
if_z                    jmp     #instructionLoop                                   '

                        cmp     cardBuffer,           #"M" wz                      ' If mounting was requested do it.
if_nz                   jmp     #instructionWait                                   ' 

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Mount Card
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

mountCard               mov     SPITiming,            #0                           ' Setup SPI parameters.
                                                                               
                        mov     counter,              #128                         ' Send out for more than 1 millisecond.                            
seventyFourClocks       call    #readSPI                                           ' 
                        djnz    counter,              #seventyFourClocks           '

' //////////////////////Go Idle State//////////////////////////////////////////////////////////////////////////////////////////

                        mov     counter,              #128                         ' Setup counter to try a few times.
                                                                    
enterIdleStateLoop      mov     SPICommandOut,        #($40 | 0)                   ' Send out command 0.
                        mov     SPIParameterOut,      #0                           '
                        movs    commandSPICRC,        #$95                         '
                        call    #commandSPI                                        '
                        call    #shutdownSPI                                       '

                        cmp     SPIResponceIn,        #1 wz                        ' Try a few times.
if_nz                   djnz    counter,              #enterIdleStateLoop          '
                        tjz     counter,              #instructionRetry            '

' //////////////////////Send Interface Condition///////////////////////////////////////////////////////////////////////////////
                        
                        mov     SPICommandOut,        #($40 | 8)                   ' Send out command 8.
                        mov     SPIParameterOut,      #$1AA                        '
                        movs    commandSPICRC,        #$87                         '
                        call    #commandSPI                                        '
                        call    #longSPI                                           '
                        call    #shutdownSPI                                       '
                        
                        test    SPIResponceIn,        #$7E wz                      ' If failure goto SD 1.X initialization.
if_nz                   jmp     #exitIdleState_SD                                  '
                                                                                
                        and     SPILongIn,            #$1FF                        ' SD 2.0 initialization.
                        cmp     SPILongIn,            #$1AA wz                     '
if_nz                   jmp     #instructionRetry                                  '
                        
' //////////////////////Send Operating Condition///////////////////////////////////////////////////////////////////////////////

exitIdleState_SD        mov     cardType,             #0                           ' Card type is MMC.

                        mov     counter,              #128                         ' Setup counter to try a few times.

exitIdleStateLoop_SD    mov     SPICommandOut,        #($40 | 55)                  ' Send out command 55.
                        mov     SPIParameterOut,      #0                           '
                        call    #commandSPI                                        '
                        call    #shutdownSPI                                       '

                        test    SPIResponceIn,        #$7E wz                      ' If failure goto MMC initialization.                                 '        
if_nz                   jmp     #exitIdleState_MMC                                 '

                        mov     SPICommandOut,        #($40 | 41)                  ' Send out command 41 with HCS bit set.
                        mov     SPIParameterOut,      HCSBitMask                   '
                        call    #commandSPI                                        '
                        call    #shutdownSPI                                       '

                        cmp     SPIResponceIn,        #0 wz                        ' Try a few times.
if_nz                   djnz    counter,              #exitIdleStateLoop_SD        '
                        tjz     counter,              #instructionRetry            '

                        djnz    cardType,             #readOCR                     ' Card type SD.

' //////////////////////Send Operating Condition///////////////////////////////////////////////////////////////////////////////
                        
exitIdleState_MMC       mov     counter,              #128                         ' Setup counter to try a few times.
                                                                              
exitIdleStateLoop_MMC   mov     SPICommandOut,        #($40 | 1)                   ' Send out command 1.
                        mov     SPIParameterOut,      #0                           '
                        call    #commandSPI                                        '
                        call    #shutdownSPI                                       '

                        cmp     SPIResponceIn,        #0 wz                        ' Try a few times.
if_nz                   djnz    counter,              #exitIdleStateLoop_MMC       '
                        tjz     counter,              #instructionRetry            '

' //////////////////////Read OCR Register//////////////////////////////////////////////////////////////////////////////////////

readOCR                 mov     SPICommandOut,        #($40 | 58)                  ' Ask the card for its OCR register.
                        mov     SPIParameterOut,      #0                           '
                        call    #commandSPI                                        '
                        call    #longSPI                                           '   
                        call    #shutdownSPI                                       '

                        tjnz    SPIResponceIn,        #instructionRetry            ' If failure abort.

                        test    SPILongIn,            OCRCheckMask wz              ' If voltage not supported abort. 
                        shl     SPILongIn,            #1 wc                        '
if_z_or_nc              jmp     #instructionRetry                                  '
                                                                              
                        shl     SPILongIn,            #1 wc                        ' SDHC supported or not.
if_c                    mov     SPIShift,             #0                           '
if_nc                   mov     SPIShift,             #9                           '

' //////////////////////Set Block Length///////////////////////////////////////////////////////////////////////////////////////
                        
                        mov     SPICommandOut,        #($40 | 16)                  ' Send out command 16.
                        mov     SPIParameterOut,      fiveHundredAndTwelve         '
                        call    #commandSPI                                        '
                        call    #shutdownSPI                                       '

                        tjnz    SPIResponceIn,        #instructionRetry            ' If failure abort.
                        
' //////////////////////Read CSD Register//////////////////////////////////////////////////////////////////////////////////////                        

                        mov     SPICommandOut,        #($40 | 9)                   ' Ask the card for its CSD register.
                        mov     SPIParameterOut,      #0                           '
                        call    #commandSPI                                        '
                                                      
                        tjnz    SPIResponceIn,        #instructionRetry            ' If failure abort.
                        call    #repsonceSPI                                       ' 
                        cmp     SPIResponceIn,        #$FE wz                      '
if_nz                   jmp     #instructionRetry                                  '
                                                      
                        mov     counter,              #16                          ' Setup to read the CSD register.
                        movd    readCSDModify,        #cardSpecificData            '
                                                      
readCSDLoop             call    #readSPI                                           ' Read the CSD register in.
readCSDModify           mov     0,                    SPIDataIn                    '
                        add     readCSDModify,        fiveHundredAndTwelve         '
                        djnz    counter,              #readCSDLoop                 '

                        call    #wordSPI                                           ' Shutdown SPI clock. 
                        call    #shutdownSPI                                       ' 
                                                      
' //////////////////////Read CID Register//////////////////////////////////////////////////////////////////////////////////////

                        mov     SPICommandOut,        #($40 | 10)                  ' Ask the card for its CID register.
                        mov     SPIParameterOut,      #0                           '
                        call    #commandSPI                                        '
                                                      
                        tjnz    SPIResponceIn,        #instructionRetry            ' If failure abort.
                        call    #repsonceSPI                                       ' 
                        cmp     SPIResponceIn,        #$FE wz                      '
if_nz                   jmp     #instructionRetry                                  '
                                                      
                        mov     counter,              #16                          ' Setup to read the CID register.
                        mov     buffer,               CUIAddress                   '
                                                                          
readCIDLoop             call    #readSPI                                           ' Read the CID register in.
                        wrbyte  SPIDataIn,            buffer                       '
                        add     buffer,               #1                           '
                        djnz    counter,              #readCIDLoop                 '

                        wrbyte  fiveHundredAndTwelve, buffer                       ' Clear the last byte for string compare.

                        call    #wordSPI                                           ' Shutdown SPI clock. 
                        call    #shutdownSPI                                       '

' //////////////////////Setup Card Variables///////////////////////////////////////////////////////////////////////////////////

                        neg     SPITiming,            #1                           ' Setup SPI parameters.

                        testn   cardType,             #0 wz, wc                    ' Determine CSD structure version.
if_nz                   test    cardSpecificData,     #$40 wc                      ' 
if_nz                   test    cardSpecificData,     #$80 wz                      '

if_nc_and_z             mov     counter,              (cardSpecificData + 6)       ' Extract card size.
if_nc_and_z             and     counter,              #$3                          '
if_nc_and_z             shl     counter,              #10                          '
if_nc_and_z             mov     buffer,               (cardSpecificData + 7)       '
if_nc_and_z             shl     buffer,               #2                           '
if_nc_and_z             mov     cardSize,             (cardSpecificData + 8)       '
if_nc_and_z             shr     cardSize,             #6                           '
if_nc_and_z             or      cardSize,             counter                      '
if_nc_and_z             or      cardSize,             buffer                       '

if_c_and_z              mov     counter,              (cardSpecificData + 7)       ' Extract card size.
if_c_and_z              and     counter,              #$3F                         '
if_c_and_z              shl     counter,              #16                          '
if_c_and_z              mov     buffer,               (cardSpecificData + 8)       '
if_c_and_z              shl     buffer,               #8                           '
if_c_and_z              mov     cardSize,             (cardSpecificData + 9)       '
if_c_and_z              or      cardSize,             counter                      '
if_c_and_z              or      cardSize,             buffer                       '

if_nc_and_z             mov     buffer,               (cardSpecificData + 9)       ' Extract card size multiplier.
if_nc_and_z             and     buffer,               #$3                          '
if_nc_and_z             shl     buffer,               #1                           '
if_nc_and_z             mov     cardSizeMultiplier,   (cardSpecificData + 10)      '
if_nc_and_z             shr     cardSizeMultiplier,   #7                           '
if_nc_and_z             or      cardSizeMultiplier,   buffer                       '
                                                      
if_nc_and_z             mov     cardReadBlockLength,  (cardSpecificData + 5)       ' Extract read block length.
if_nc_and_z             and     cardReadBlockLength,  #$F                          '
                                                                             
if_nc_and_z             sub     cardReadBlockLength,  #9                           ' Compute card sector count for 1.0 CSD.
if_nc_and_z             add     cardSizeMultiplier,   #2                           '     
if_z                    add     cardSize,             #1                           '
if_nc_and_z             shl     cardSize,             cardReadBlockLength          '                        
if_nc_and_z             shl     cardSize,             cardSizeMultiplier           '
                                                      
if_c_and_z              shl     cardSize,             #10                          ' Compute card sector count for 2.0 CSD. 

                        max     cardSize,             maxPositiveInteger           ' Limit maximum partition size.

if_nz                   neg     cardSize,             #1                           ' Unknown CSD structure. Card size to -1.   

                        wrlong  cardSize,             CSCAddress                   ' Update Card Size.

                        mov     cardSizeMinusOne,     cardSize                     ' Compute maximum addressable sector.
                        sub     cardSizeMinusOne,     #1                           '

                        neg     cardMounted,          #1                           ' Return.
                        jmp     #instructionLoop                                   '
                               
' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Read Block
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        
readBlock               rdlong  SPIParameterOut,      CSAAddress                   ' Read a block.  
                        max     SPIParameterOut,      cardSizeMinusOne             '
                        shl     SPIParameterOut,      SPIShift                     '
                        mov     SPICommandOut,        #($40 | 17)                  '
                        call    #commandSPI                                        '

                        tjnz    SPIResponceIn,        #instructionRetry            ' If failure abort.
                        call    #repsonceSPI                                       '     
                        cmp     SPIResponceIn,        #$FE wz                      '
if_nz                   jmp     #instructionRetry                                  '
                                                      
                        mov     counter,              fiveHundredAndTwelve         ' Setup loop.
readBlockModify         rdlong  buffer,               CBAAddress                   '
                                                                                  
readBlockLoop           call    #readSPI                                           ' Read data into memory.
                        wrbyte  SPIDataIn,            buffer                       '
                        add     buffer,               #1                           '               
                        djnz    counter,              #readBlockLoop               '                           

                        call    #wordSPI                                           ' Shutdown SPI clock.
                        call    #shutdownSPI                                       ' 

readBlock_ret           jmp     #instructionLoop                                   ' Return. Becomes RET when rebooting.
                        
' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Write Block
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

writeBlock              rdlong  SPIParameterOut,      CSAAddress                   ' Write a block.
                        max     SPIParameterOut,      cardSizeMinusOne             '
                        shl     SPIParameterOut,      SPIShift                     '
                        mov     SPICommandOut,        #($40 | 24)                  '
                        call    #commandSPI                                        '

                        tjnz    SPIResponceIn,        #instructionRetry            ' If failure abort.

                        mov     SPIDataOut,           #$FE                         ' Send start of data token.
                        call    #writeSPI                                          '

                        mov     counter,              fiveHundredAndTwelve         ' Setup loop.
                        rdlong  buffer,               CBAAddress                   '

writeBlockLoop          rdbyte  SPIDataOut,           buffer                       ' Write data out from memory.
                        add     buffer,               #1                           '
                        call    #writeSPI                                          '
                        djnz    counter,              #writeBlockLoop              '          

                        call    #wordSPI                                           ' Write out the 16 bit CRC.

                        call    #repsonceSPI                                       ' If failure abort.
                        and     SPIDataIn,            #$1F                         '
                        cmp     SPIDataIn,            #$5 wz                       '
if_nz                   jmp     #instructionRetry                                  '
                                                                                    
                        call    #cardBusy                                          ' Shutdown SPI clock.
                        call    #shutdownSPI                                       ' 

                        jmp     #instructionLoop                                   ' Return.
                        
' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Reboot Chip
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                        
rebootChip              rdlong  buffer,               CBAAddress                   ' Check to make sure its a reboot.
                        tjnz    buffer,               #instructionError            '
                        
' //////////////////////Shutdown Cogs////////////////////////////////////////////////////////////////////////////////////////// 
                        
                        mov     counter,              #8                           ' Setup cog stop loop.
                        cogid   buffer                                             '
                        
rebootCogLoop           sub     counter,              #1                           ' Stop all cogs but this one.
                        cmp     counter,              buffer wz                    '
if_nz                   cogstop counter                                            '
                        tjnz    counter,              #rebootCogLoop               '
                        
' //////////////////////Setup Memory///////////////////////////////////////////////////////////////////////////////////////////
                        
                        mov     counter,              #64                          ' Setup to grab all sector addresses.
                        rdlong  buffer,               CSAAddress                   '

rebootSectorLoadLoop    rdlong  cardRebootSectors,    buffer                       ' Get all addresses of the 64 sectors. 
                        add     buffer,               #4                           '
                        add     rebootSectorLoadLoop, fiveHundredAndTwelve         '
                        djnz    counter,              #rebootSectorLoadLoop        '
                        
' //////////////////////Clear Memory///////////////////////////////////////////////////////////////////////////////////////////

                        mov     counter,              fiveHundredAndTwelve         ' Clear all memory. Pointer at 0.
                        shl     counter,              #6                           '
                        mov     buffer,               #0                           '   
rebootCodeClearLoop     sub     counter,              #4                           '
                        wrlong  buffer,               counter                      '
                        tjnz    counter,              #rebootCodeClearLoop         ' 
                                     
' //////////////////////Fill Memory////////////////////////////////////////////////////////////////////////////////////////////

                        mov     readBlock,            #0                           ' Fill these two commands with NOPs.
                        mov     readBlockModify,      #0                           '
                                                                               
                        mov     cardCounter,          #64                          ' Ready to fill all memory. Pointer at 0.
                        
rebootCodeFillLoop      mov     SPIParameterOut,      cardRebootSectors            ' Reuse read block code. Finish if 0.
                        tjz     SPIParameterOut,      #rebootReady                 '
                        add     rebootCodeFillLoop,   #1                           '
                        call    #readBlock                                         '
                        djnz    cardCounter,          #rebootCodeFillLoop          '

' //////////////////////Boot Interpreter///////////////////////////////////////////////////////////////////////////////////////
                        
rebootReady             rdword  buffer,               #$A                          ' Setup the stack markers.
                        sub     buffer,               #4                           '
                        wrlong  rebootStackMarker,    buffer                       '
                        sub     buffer,               #4                           '
                        wrlong  rebootStackMarker,    buffer                       '

                        rdbyte  buffer,               #$4                          ' Switch to new clock mode.
                        clkset  buffer                                             '

                        coginit rebootInterpreter                                  ' Restart running new code.

                        cogid   buffer                                             ' Shutdown.
                        cogstop buffer                                             '

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Card Status
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cardStatus              mov     SPICommandOut,        #($40 | 13)                  ' Send out command 13.
                        mov     SPIParameterOut,      #0                           '
                        call    #commandSPI                                        '
                        call    #byteSPI                                           '
                        call    #shutdownSPI                                       '

                        or      SPIResponceIn,        SPILongIn wz                 ' Unmount the card on failure.
                        muxz    cardMounted,          #1                           '

cardStatus_ret          ret                                                        ' Return.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Card Busy
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cardBusy                mov     counter,              writeTimeout                 ' Setup loop.

cardBusyLoop            call    #readSPI                                           ' Wait until card is not busy.
                        cmp     SPIDataIn,            #0 wz                        '
if_z                    djnz    counter,              #cardBusyLoop                '
                        tjz     counter,              #instructionRetry            '

cardBusy_ret            ret                                                        ' Return.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Command SPI
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

commandSPI              andn    outa,                 chipSelectPin                ' Activate the SPI bus.  
                        call    #readSPI                                           '
                                                   
                        mov     SPIDataOut,           SPICommandOut                ' Send out command.
                        call    #writeSPI                                          '
                                                   
                        movs    writeSPI,             #32                          ' Send out parameter.
                        mov     SPIDataOut,           SPIParameterOut              '
                        call    #writeSPI                                          '
                        movs    writeSPI,             #8                           '
                                                   
commandSPICRC           mov     SPIDataOut,           #0                           ' Send out CRC token.
                        call    #writeSPI                                          '

                        call    #repsonceSPI                                       ' Read in responce.
                                                                               
commandSPI_ret          ret                                                        ' Return.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Responce SPI
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

repsonceSPI             mov     SPIResponceIn,        readTimeout                  ' Setup responce poll counter.
                        
repsonceSPILoop         call    #readSPI                                           ' Poll for responce.
                        cmp     SPIDataIn,            #$FF wz                      '
if_z                    djnz    SPIResponceIn,        #repsonceSPILoop             '
                                                    
                        mov     SPIResponceIn,        SPIDataIn                    ' Move responce into return value.
                                                                         
repsonceSPI_ret         ret                                                        ' Return.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Long SPI
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

longSPI                 add     readSPI,              #16                          ' Read in 32, 16, or 8 bits.
wordSPI                 add     readSPI,              #8                           ' 
byteSPI                 call    #readSPI                                           '
                        movs    readSPI,              #8                           '
                        
                        mov     SPILongIn,            SPIDataIn                    ' Move long into return value.
                        
byteSPI_ret                                                                        ' Return.
wordSPI_ret                                                                        '
longSPI_ret             ret                                                        ' 

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Shutdown SPI
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

shutdownSPI             call    #readSPI                                           ' Shutdown the SPI bus.
                        or      outa,                 chipSelectPin                '
                        call    #readSPI                                           '
                                                                          
shutdownSPI_ret         ret                                                        ' Return.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Read SPI
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                                 
readSPI                 mov     SPICounter,           #8                           ' Setup counter to read in 1 - 32 bits.
                        mov     SPIDataIn,            #0 wc                        '

readSPIAgain            mov     phsa,                 #0                           ' Start clock low.
                        tjnz    SPITiming,            #readSPISpeed                '

' //////////////////////Slow Reading///////////////////////////////////////////////////////////////////////////////////////////

                        movi    frqa,                 #%0000_0000_1                ' Start the clock - read 1 .. 32 bits.  

readSPILoop             waitpne clockPin,             clockPin                     ' Get bit.
                        rcl     SPIDataIn,            #1                           '
                        waitpeq clockPin,             clockPin                     '
                        test    dataOutPin,           ina wc                       '
                        
                        djnz    SPICounter,           #readSPILoop                 ' Loop until done.
                        jmp     #readSPIFinish                                     '
                        
' //////////////////////Fast Reading///////////////////////////////////////////////////////////////////////////////////////////

readSPISpeed            movi    frqa,                 #%0010_0000_0                ' Start the clock - read 8 bits.

                        test    dataOutPin,           ina wc                       ' Read in data.
                        rcl     SPIDataIn,            #1                           '
                        test    dataOutPin,           ina wc                       '
                        rcl     SPIDataIn,            #1                           '
                        test    dataOutPin,           ina wc                       '
                        rcl     SPIDataIn,            #1                           '
                        test    dataOutPin,           ina wc                       '
                        rcl     SPIDataIn,            #1                           '
                        test    dataOutPin,           ina wc                       '
                        rcl     SPIDataIn,            #1                           '
                        test    dataOutPin,           ina wc                       '
                        rcl     SPIDataIn,            #1                           '
                        test    dataOutPin,           ina wc                       '
                        rcl     SPIDataIn,            #1                           '
                        test    dataOutPin,           ina wc                       '

' //////////////////////Finish Up//////////////////////////////////////////////////////////////////////////////////////////////
                        
readSPIFinish           mov     frqa,                 #0                           ' Stop the clock.
                        rcl     SPIDataIn,            #1                           '

                        cmpsub  SPICounter,           #8                           ' Read in any remaining bits.
                        tjnz    SPICounter,           #readSPIAgain                '
                                                     
readSPI_ret             ret                                                        ' Return.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Write SPI
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

writeSPI                mov     SPICounter,           #8                           ' Setup counter to write out 1 - 32 bits.
                        ror     SPIDataOut,           SPICounter                   '                             

writeSPIAgain           mov     phsa,                 #0                           ' Start clock low.
                        tjnz    SPITiming,            #writeSPISpeed               '
                        
' //////////////////////Slow Writing//////////////////////////////////////////////////////////////////////////////////////////

                        movi    frqa,                 #%0000_0000_1                ' Start the clock - write 1 .. 32 bits.

writeSPILoop            shl     SPIDataOut,           #1 wc                        ' Set bit.
                        waitpne clockPin,             clockPin                     '
                        muxc    outa,                 dataInPin                    ' 
                        waitpeq clockPin,             clockPin                     ' 

                        djnz    SPICounter,           #writeSPILoop                ' Loop until done.
                        jmp     #writeSPIFinish                                    '

' //////////////////////Fast Writing//////////////////////////////////////////////////////////////////////////////////////////

writeSPISpeed           shl     SPIDataOut,           #1 wc                        ' Write out data.
                        muxc    outa,                 dataInPin                    '

                        movi    frqa,                 #%0010_0000_0                ' Start the clock - write 8 bits.
                           
                        shl     SPIDataOut,           #1 wc                        ' Write out data.
                        muxc    outa,                 dataInPin                    '
                        shl     SPIDataOut,           #1 wc                        '
                        muxc    outa,                 dataInPin                    '
                        shl     SPIDataOut,           #1 wc                        '
                        muxc    outa,                 dataInPin                    '
                        shl     SPIDataOut,           #1 wc                        '
                        muxc    outa,                 dataInPin                    '
                        shl     SPIDataOut,           #1 wc                        '
                        muxc    outa,                 dataInPin                    '
                        shl     SPIDataOut,           #1 wc                        '
                        muxc    outa,                 dataInPin                    '
                        shl     SPIDataOut,           #1 wc                        '
                        muxc    outa,                 dataInPin                    '
                        
' //////////////////////Finish Up//////////////////////////////////////////////////////////////////////////////////////////////

writeSPIFinish          mov     frqa,                 #0                           ' Stop the clock.
                        or      outa,                 dataInPin                    ' 

                        cmpsub  SPICounter,           #8                           ' Write out any remaining bits.
                        tjnz    SPICounter,           #writeSPIAgain               '
                       
writeSPI_ret            ret                                                        ' Return.

' ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
'                       Data
' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////                        

cardMounted             long    0                                                   

' //////////////////////Constants//////////////////////////////////////////////////////////////////////////////////////////////

fiveHundredAndTwelve    long    $200                                               ' Constant 512.
maxPositiveInteger      long    $7FFFFFFF                                          ' Constant 2,147,483,647.

OCRCheckMask            long    %00_000000_00110000_00000000_00000000              ' Parameter check mask for OCR bits.
HCSBitMask              long    %01_000000_00000000_00000000_00000000              ' Parameter bit mask for HCS bit.
                                                                              
rebootInterpreter       long    ($0001 << 18) | ($3C01 << 4)                       ' Spin interpreter text boot information. 
rebootStackMarker       long    $FFF9FFFF                                          ' Spin interpreter stack boot information.  
                     
' //////////////////////Configuration Settings/////////////////////////////////////////////////////////////////////////////////

readTimeout             long    0                                                  ' 100 millisecond timeout.
writeTimeout            long    0                                                  ' 250 millisecond timeout. 

dataOutCounterSetup     long    0                                                  ' Data out control.
clockCounterSetup       long    0                                                  ' Clock control.
dataInCounterSetup      long    0                                                  ' Data in control.

' //////////////////////Pin Masks//////////////////////////////////////////////////////////////////////////////////////////////

dataOutPin              long    0                                                  
clockPin                long    0                                                  
dataInPin               long    0                                                  
chipSelectPin           long    0                                                  

' //////////////////////Addresses//////////////////////////////////////////////////////////////////////////////////////////////
                                          
CBAAddress              long    0                                                  
CSAAddress              long    0
CSCAddress              long    0
CCFAddress              long    0                                                
CEFAddress              long    0
CUIAddress              long    0                                                

' //////////////////////Run Time Variables/////////////////////////////////////////////////////////////////////////////////////

buffer                  res     1                                              
counter                 res     1                                               

' //////////////////////Card Variables/////////////////////////////////////////////////////////////////////////////////////////

cardBuffer              res     1                                             
cardCounter             res     1

cardType                res     1                                              
cardSize                res     1                                            

cardSizeMultiplier      res     1                                             
cardSizeMinusOne        res     1                                          

cardReadBlockLength     res     1                                     
cardWriteBlockLength    res     1                                          

cardSpecificData        res     16
cardRebootSectors       res     64                                       

' //////////////////////SPI Variables//////////////////////////////////////////////////////////////////////////////////////////

SPICommandOut           res     1
SPIParameterOut         res     1
SPIResponceIn           res     1
SPILongIn               res     1

SPIShift                res     1
SPITiming               res     1

SPIDataIn               res     1
SPIDataOut              res     1

SPIBuffer               res     1
SPICounter              res     1

' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                        fit     496

DAT

' //////////////////////Variable Array/////////////////////////////////////////////////////////////////////////////////////////

cardBlockAddress        long 0 ' Address of the block in memory to read bytes from and write bytes to. 
cardSectorAddress       long 0 ' Address of the sector on the card to write bytes to and read bytes from.

cardSectorCount         long 0 ' The secure digital card sector count.
cardCommandFlag         byte 0 ' The secure digital card driver function command flag.
cardErrorFlag           byte 0 ' The secure digital card driver function result flag.

cardUniqueID            byte 0[17] ' The secure digital card identification number.
cardLockID              byte 0 ' The secure digital card driver lock number.
cardCogID               byte 0 ' The secure digital card driver cog number.

DAT

' //////////////////////String Array///////////////////////////////////////////////////////////////////////////////////////////

dot                     byte ".          ", 0
dotdot                  byte "..         ", 0

FSCorrupted             byte "File System Corrupted", 0 
FSUnsupported           byte "File System Unsupported", 0
fileOrDirectoryNotFound byte "File Or Directory Not Found", 0

fileNotFound            byte "File Not Found", 0
directoryNotFound       byte "Directory Not Found", 0
fileIsReadOnly          byte "File Is Read Only", 0

' /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                 
{{

┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                                   TERMS OF USE: MIT License                                                 │                                                            
├─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation   │ 
│files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,   │
│modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the        │
│Software is furnished to do so, subject to the following conditions:                                                         │         
│                                                                                                                             │
│The above copyright notice and this permission notice shall be included in all copies or substantial portions of the         │
│Software.                                                                                                                    │
│                                                                                                                             │
│THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE         │
│WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR        │
│COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,  │
│ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                        │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
}}