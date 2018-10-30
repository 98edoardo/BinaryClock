# BinaryClock

## Come impostare l'orario
Per entrare in modalità modificare cliccare il bottone `MODIFICA` e scegliere la riga interessata (cliccando nuovamente dopo selezionato la riga dei secondi si uscirà dalla modalità), succesivamente utilizzare il bottone `INCREMENTO` per incrementare la riga selezionata di 1.

## [MODALITA' 1] Simple clock
**In questa modalità NON è richiesto lo Shield Ethernet.**
Nel file `OrologioBinario.ino` modificare la costante `CLOCK_MODE` a 0.
Collegare il bottone `MODIFICA` al PIN 2, il bottone `INCREMENTO` al PIN 3.

## [MODALITA' 2] NTP clock
**In questa modalità è richiesto lo Shield Ethernet.**
Nel file `OrologioBinario.ino` modificare la costante `CLOCK_MODE` a 1.

Collegare il bottone `MODIFICA` al PIN 2, il bottone `INCREMENTO` al PIN 3.

Aggiustare il fuso orario agendo sui bottoni.

## [MODALITA' 3] SOCKET clock
**In questa modalità è richiesto lo Shield Ethernet.**
Nel file `OrologioBinario.ino` modificare la costante `CLOCK_MODE` a 2.

Collegare il bottone `MODIFICA` al PIN 2, il bottone `INCREMENTO` al PIN 3.

Inviare via telnet (porta 3306) il timestamp attuale seguito da un punto e virgola per impostare l'orario, es. `1540910852;`.
Inviare via telnet (porta 3306) il carattere S per incrementare i secondi di 1.

## DEBUG
E' possibile richiedere l'invio via seriale dell'orario attuale e dell'offset impostando la cosante `DEBUG` a 1.
