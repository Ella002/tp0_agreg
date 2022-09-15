(** Repartiteur fournit un repartiteur de charge parmi des serveurs *)

(** une tache est decrite par trois parametres:
    la duree, la charge, et une donnee *)
type 'a tache = (int * float * 'a)

(** l'etat d'un serveur est decrit par la liste des taches
    en cours d'execution *)
type 'a serveur = 'a tache list

(** l'etat de l'ensemble des serveurs est decrit par la liste de l'etat
    des serveurs individuels *)
type 'a etat_serveurs = 'a serveur list

(** [calcul_charge_serveur serveur] additionne les charges des taches du serveur *)
val calcul_charge_serveur : 'a serveur -> float

(** [ajout_tache_serveur tache etat_serveurs id] ajoute la tache au serveur
    numero id de la liste *)
val ajout_tache_serveur : 'a tache -> 'a etat_serveurs -> int -> 'a etat_serveurs

(** [calcul_surcharge_serveur tache serveur] calcul la surcharge de serveur, si on
    y executait tache *)
val calcul_surcharge_serveur : 'a tache -> 'a serveur -> float

(** [repartiteur ~nb_max tache etat_serveurs] choisit quel serveur va executer
    tache, selon les criteres suivants:
    - s'il y a au moins un serveur qui peut l'executer sans surcharge,
    on choisit le premier pour lequel la charge restante est minimale
    - sinon, si le nombre de serveurs n'est pas atteint (ou illimite),
    on cree un nouveau serveur
    - sinon on selectionne le serveur pour lequel la surcharge serait minimale
 *)
val repartiteur : ?nb_max:int -> 'a tache -> 'a etat_serveurs -> 'a etat_serveurs
