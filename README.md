EECE381-EBS-DE2repo
===================

Electronic Beat Sequencer interfaced with Android (DE2 Section)
All of the Qsys, VHDL and C files used in this projects are stored in this repo. The whole project files are not upoaded due to the  compatibilities that different systems have. In order to get the project to test, use or modify: 
Quartus projects has to be created and then Qsys and VHDL files should be added to the project. 
The Nios1 fodler contains the C files for core 1. The Nios2 fodler contans the C files for  second core. 

Introduction to the project: 
  By allowing multiple users to interact with the same de2 board’s loop users could, in a sense, “jam” on the de2 board. Each user would be able to select up to 4 unique samples that they were able to control the placement of within the de2 board’s master loop, mimicking the ability of having a real instrument. Having 2 users interacting with 4 unique samples all being compiled and played back in real time on the de2 results in a lot of processing! Too much for just one core to handle along with wireless communication and VGA output. This was the motivation behind implementing dual core functionality on the de2 board. One core for doing all .wav file addition and another for managing which samples were to be combined and the wireless message passing between the de2 and android user interfaces. This allowed us to maintain a constant loop length playback, smooth graphics updating on the VGA, and reliable message handling from Android devices, along with being able to play up to 8 .wav samples simultaneously. After achieving this, we decided that our project was not heavy enough in Android processing, so we integrated a native Android step sequencer project from github into our own. After the successful implementation of this, the user had the ability to “practice” their sequencing on their own device before showing up to the “jam” where they can program sequencers with friends within the same app. 
 
 At the high level design, the android side of the project is responsible for interacting with user and communicating with the DE2 board as well as providing a local android beat sequencer. On the DE2 board, there are two NIOS II softcore. First core is mainly doing computation and the second core is  mainly responsible for communicating with the Android devices and updating the VGA monitors.
