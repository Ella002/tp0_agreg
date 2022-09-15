type 'a tache = (int * float * 'a)
type 'a serveur = 'a tache list
type 'a etat_serveurs = 'a serveur list

let calcul_charge_serveur serveur =
  (* on a un serveur, soit une liste de taches, et on calcule la charge
     de ce serveur en additionnant les charges des taches *)
  List.fold_left
    (fun acc (_, charge, _) -> acc +. charge)
    0.
    serveur

let ajout_tache_serveur tache etat_serveurs id =
  (* etant donne id un index de la liste des serveurs,
     on ajoute tache aux taches executees par le serveur a cet index
     dans la liste *)
  assert (id < List.length etat_serveurs) ;
  let rec aux cpt acc serveurs =
    match serveurs with
    | [] -> assert false
    | s::tl when cpt == id ->
       (* on ajoute la tache a la liste des taches du serveur *)
       let new_serveur = tache::s in
       (* on concatene la liste des serveurs deja vus, notre nouveau serveur,
          et le reste des serveurs.
          NB: on utilise rev_append pour preserver l'ordre des serveurs *)
       List.rev_append (new_serveur::acc) tl
    | s::tl ->
       (* on continue a parcourir la liste en cherchant notre chercheur *)
       aux (cpt+1) (s::acc) tl
  in
  aux 0 [] etat_serveurs

let calcul_surcharge_serveur (duree_nv, charge_nv, _) serveur =
  (* on a un serveur, soit une liste de taches, et on calcule la surcharge
     causee par l'ajout de tache en calculant la charge pour chaque seconde
     de la duree de la nouvelle tache *)
  let ajout_charge_tache charge_par_sec (duree, charge, _) =
    (* on a la charge par seconde calculee jusque la, et une tache.
       on veut aujouter la charge de la tache a chaque seconde de sa duree *)
    let rec aux cpt acc cps =
      match cps with
      | [] ->
         (* on a atteint la fin de cps, soit la duree de la nouvelle tache *)
         List.rev acc
      | x::tl when cpt < duree ->
         (* si la tache n'est pas terminee, on ajoute sa charge *)
         aux (cpt+1) (x+.charge::acc) tl
      | _ ->
         (* la tache est terminee, on concatene la partie incrementee et le reste *)
         List.rev_append acc cps
    in
    aux 0 [] charge_par_sec
  in
  (* on initialise la charge pendant la nouvelle tache *)
  let charge_par_sec = List.init duree_nv (fun _ -> charge_nv) in
  (* on ajoute la charge des taches du serveur *)
  let charge_par_sec =
    List.fold_left ajout_charge_tache charge_par_sec serveur
  in
  (* pour chaque seconde, si la charge depasse 1,
     on ajoute le depassement au calcul de la surcharge *)
  let surcharge =
    List.fold_left
      (fun acc charge -> if charge > 1. then acc +. (charge-.1.) else acc)
      0.
      charge_par_sec
  in
  surcharge

let repartiteur ?(nb_max = 0) (duree, charge, data) etat_serveurs =
  (* on cherche parmi la liste des serveurs, le premier qui a suffisamment de
     charge pour faire tourner la tache, mais ou il reste le moins de charge *)
  let rec tv_charge_min id_min charge_min id serveurs =
    (* on calcule la charge restante de chaque serveur, et on cherche le premier
       de charge minimale *)
    match serveurs with
    | [] -> id_min
    | s::tl ->
       (* on calcule la charge restante pour ce serveur *)
       let charge_restante = 1. -. calcul_charge_serveur s in
       (* si elle est superieure a la charge de la tache,
          mais inferieure au minimum trouve jusque la,
          c'est notre nouveau candidat *)
       if charge_restante >= charge && charge_restante < charge_min then
         tv_charge_min id charge_restante (id+1) tl
       else
         tv_charge_min id_min charge_min (id+1) tl
  in
  let id_min = tv_charge_min (-1) 1.1 0 etat_serveurs in
  if id_min >= 0 then
    (* si on a trouve un serveur sur lequel executer la tache,
       on l'ajoute a ce serveur *)
    ajout_tache_serveur (duree, charge, data) etat_serveurs id_min
  else if nb_max == 0 || List.length etat_serveurs < nb_max then
    (* si non, mais qu'on peut encore ajouter un serveur, on le fait *)
    [(duree, charge, data)]::etat_serveurs
  else
    (* sinon, on doit faire une surcharge:
       on cherche le serveur pour lequel la surcharge est minimale *)
    let rec tv_surcharge_min id_min surcharge_min id serveurs =
      match serveurs with
      | [] -> id_min
      | s::tl ->
         let surcharge = calcul_surcharge_serveur (duree, charge, data) s in
         if surcharge < surcharge_min then
           tv_surcharge_min id surcharge (id+1) tl
         else
           tv_surcharge_min id_min surcharge_min (id+1) tl
    in
    (* on initialise avec la surcharge du serveur 0 *)
    let id_min = 0 in
    let surcharge_min =
      calcul_surcharge_serveur (duree, charge, data) (List.hd etat_serveurs)
    in
    (* on parcourt les serveurs pour trouver celui de surcharge minimale,
       et on lui ajoute la tache *)
    let id_min =
      tv_surcharge_min id_min surcharge_min 1 (List.tl etat_serveurs)
    in
    ajout_tache_serveur (duree, charge, data) etat_serveurs id_min
