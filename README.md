# Boid - Simulazione di Robotica di Gruppo

## 1 Introduzione

Nel 1987 Craig Reynolds pubblicò il modello Boid che simula il comportamento che si osserva negli stormi di uccelli. Il nome boid deriva da “bird-oid object”, cioè un oggetto simile a un uccello. L’utilità del modello è l’uso che se ne può fare nell’animazione invece di mimare e creare manualmente gli elementi dello stormo in movimento. Ad esempio, un’implementazione di questo modello è stata usata per l’animazione del branco di gnu e degli uccelli in cielo durante l’iconica sequenza del calpestio nel film The Lion King della Disney. Se siete interessati a una spiegazione più approfondita potete fare riferimento all’articolo originale di Craig Reynold “Flocks, Herds, and Schools: A Distributed Behavioral Model” che introduce il modello di Boid. L’articolo fornisce una spiegazione in profondità del modello e delle sue regole.

Il modello da implementare descrive il movimento di un insieme di robot puntiformi immersi in uno spazio bidimensionale, ognuno dotato di una sua posizione e velocità di movimento. Ogni robot presente nel progetto è un boid, cioè un agente autonomo che si muove guidato da tre semplici regole:

### Coesione (Figura 1a):
I boid cercano di mantenere lo stormo unito avvicinandosi lentamente al centro dello stormo, che viene calcolato come media delle posizioni degli
altri robot. Questa regola incoraggia i boid a restare vicini come gruppo e promuove un senso di unità nello stormo.
### Separazione (Figura 1b): 
Ogni boid, se troppo vicino ad un altro, cercherà di allontanarsi dalla “folla”. Questa regola evita che si verifichino delle collisioni e assicura che i boid non si spingano troppo vicino gli uni agli altri.
### Allineamento (Figura 1c): 
Ogni boid cerca di adattare la propria velocità (in direzione, verso e modulo) a quella degli altri componenti dello stormo. Questa regola promuove un movimento nella stessa direzione dei vicini, contribuendo a un moto di gruppo più coesivo.

<div align="center">
  <img src="https://github.com/Brook06/Boid/blob/main/images/Regole%20del%20modello%20Boid.jpeg" alt="Test Image">
</div>

## Struttura del Progetto

Il progetto è organizzato in modo modulare:

- **`build`:** Contiene i file generati durante il processo di compilazione ed il file visualizer.py.
- **`include`:** Contiene il file Robot.h definizione della classe Robot.
- **`libs`:** Include libreria Robot.cpp utilizzata nel progetto.
- **`src`:** Contiene i codici sorgenti principali del progetto.
- **`visualizer`:** Include file o script relativi alla visualizzazione dei dati.

## Come Compilare

Per compilare il progetto, seguire questi semplici passaggi:
Navigare nella cartella `build`.

   `cd build`

   `cmake ..`

   `make`


## Come eseguire

se si vuole eseguire la soluzione con variabili condizione :

   `./src/main`

altrimenti la soluzione con mutex :

   `./src/main1`

## Come Visualizzare

Primo metodo dentro cartella **`build`:**
  
  `python3 visualizer.py`

Secondo metodo dentro la cartella **`visualizer`:**

   `cd ..`

   `cd visualizer`

   `python3 visualizer.py`

Se si vuole utilizzare i file presenti nella cartella visualizer, bisogno modificare il nome del file nel seguente nome:
  **`coordinates.txt`**
  
I numeri dopo il  **`coordinates.txt`** stanno ad indicare il numero di Robot (Boid) coinvolti in quella simulazione.
