# EscorpioEvo16 Can Analyzer
##Introduction
This program is used with the Can Analyzer function provided by the [EscorpioEvo16-Dashboard](https://github.com/DavideMalvezzi/EscorpioEvo16-Dashboard).
The software connectcs directly to the Dashboard serial port or bluetooth module and parses all the received can packets and shows them.
The packets visualization could be chronologically (all the packets are shown) or uniquely (only the last packet is shown for each can id). Its also possible to filter the shown packets by the can id with the filter interface.
Also is possible to send custom packets through the TX interface.
All the channels information are saved and loaded from a local SQLite database.

##Not finished
There are two modules not finished yet.
The first one is the record module in charge of save all the packets inside the database and manage the previous recorded data.
The second is the streaming module in charge of send all the packets throgh the local area network to other Can Analyzer software to share 
the can bus traffic in real time.
