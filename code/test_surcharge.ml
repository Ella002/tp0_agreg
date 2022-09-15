open Utils

let test_sujet () =
  (* Exemple du sujet:
     On a un serveur qui a une tache de .25 pendant 1 seconde,
     et une de .25 pendant 2 secondes.
     On ajoute une tache de charge 1 pendant 3 secondes.
     Surcharge = .5 (seconde 1) + .25 (seconde 2) = .75
   *)
  let tache = (3, 1., 42) in
  let serveur = [(1, 0.25, 11); (2, 0.25, 8)] in
  let res = Repartiteur.calcul_surcharge_serveur tache serveur in
  compare_surcharge "test_sujet" ~res ~exp:0.75

let test_2 () =
  (* Deuxieme exemple:
     Serveur qui a tache de .7 pendant 1 seconde,
     et une de .05 pendant 1 seconde.
     On ajoute une tache de 1 pendant 3 secondes.
     Surcharge = .75 (seconde 1)
   *)
  let tache = (3, 1., 42) in
  let serveur = [(1, 0.7, 11); (4, 0.05, 8)] in
  let res = Repartiteur.calcul_surcharge_serveur tache serveur in
  compare_surcharge "test_2" ~res ~exp:0.85

let test_no_surcharge () =
  (* Exemple sans surcharge:
     Serveur qui a tache de .4 pendant 2 secondes.
     On ajoute une tache de .5 pendant 3 secondes.
     Surcharge = 0
   *)
  let tache = (1, 0.5, 42) in
  let serveur = [(2, 0.4, 88)] in
  let res = Repartiteur.calcul_surcharge_serveur tache serveur in
  compare_surcharge "test_no_surcharge" ~res ~exp:0.

let () =
  test_sujet () ;
  test_2 () ;
  test_no_surcharge ()
