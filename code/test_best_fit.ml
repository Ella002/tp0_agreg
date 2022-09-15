open Utils

let test_sujet () =
  (* Exemple du sujet:
     - tache de charge .5
     - serveur1 de charge restante .4
     - serveur2 de charge restante .6
     - serveur3 de charge restante .7
     => on va ajouter au serveur2 *)
  let tache = (1, 0.5, 42) in
  let serveur1 = [(2, 0.6, 88)] in
  let serveur2 = [(1, 0.2, 11); (19, 0.2, 8)] in
  let serveur3 = [(1, 0.3, 27)] in
  let res = Repartiteur.repartiteur tache [serveur1; serveur2; serveur3] in
  let exp = [ serveur1 ; tache::serveur2 ; serveur3 ]
  in
  compare_liste_serveurs "test_sujet" ~res ~exp

let test_nv_serveur () =
  (* Exemple avec nouveau serveur:
     - tache de charge .5
     - serveur1 de charge restante .4
     => besoin d'un nouveau serveur *)
  let tache = (1, 0.5, 42) in
  let serveur1 = [(2, 0.6, 88)] in
  let res = Repartiteur.repartiteur tache [serveur1] in
  let exp = [[tache] ; serveur1] in
  compare_liste_serveurs "test_nv_serveur" ~res ~exp

let test_2_options () =
  (* Exemple ou ils y a deux serveurs avec la meme charge restante:
     - tache de charge .4
     - serveur1 de charge restante .5
     - serveur2 de charge restante .5
     => on selectionne le premier, soit serveur1 *)
  let tache = (1, 0.5, 42) in
  let serveur1 = [(2, 0.5, 88)] in
  let serveur2 = [(1, 0.2, 11); (19, 0.3, 8)] in
  let res = Repartiteur.repartiteur tache [serveur1 ; serveur2] in
  let exp = [tache::serveur1 ; serveur2] in
  compare_liste_serveurs "test_2_options" ~res ~exp

let test_liste_vide () =
  (* Exemple ou la liste des serveurs est vide *)
  let tache = (1, 0.5, 42) in
  let res = Repartiteur.repartiteur tache [] in
  let exp = [[tache]] in
  compare_liste_serveurs "test_liste_vide" ~res ~exp

let test_serveur_vide () =
  (* Exemple ou il y a un serveur dont la liste de taches est vide *)
  let tache = (1, 0.5, 42) in
  let serveur1 = [(2, 0.6, 88)] in
  let serveur2 = [] in
  let res = Repartiteur.repartiteur tache [serveur1; serveur2] in
  let exp = [serveur1 ; tache::serveur2] in
  compare_liste_serveurs "test_serveur_vide" ~res ~exp

let () =
  test_sujet () ;
  test_nv_serveur () ;
  test_2_options () ;
  test_liste_vide () ;
  test_serveur_vide ()
