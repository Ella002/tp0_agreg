open Utils

let test_sujet () =
  let tache = (3, 1., 42) in
  let serveur = [(1, 0.25, 11); (2, 0.25, 8)] in
  let res = Repartiteur.repartiteur ~nb_max:1 tache [serveur] in
  let exp = [[(3, 1., 42); (1, 0.25, 11); (2, 0.25, 8)]] in
  compare_liste_serveurs "test_sujet" ~res ~exp

let test_simple () =
  let tache = (3, 1., 42) in
  let serveur1 = [(1, 0.25, 11); (2, 0.25, 8)] in
  let serveur2 = [(1, 0.1, 11); (2, 0.25, 8)] in
  let res = Repartiteur.repartiteur ~nb_max:2 tache [serveur1; serveur2] in
  let exp = [[(1, 0.25, 11); (2, 0.25, 8)]; [(3, 1., 42); (1, 0.1, 11); (2, 0.25, 8)]] in
  compare_liste_serveurs "test_simple" ~res ~exp

let test_2_options () =
  let tache = (3, 1., 42) in
  let serveur1 = [(1, 0.25, 11); (2, 0.25, 8)] in
  let serveur2 = [(1, 0.70, 11); (1, 0.05, 8)] in
  let res = Repartiteur.repartiteur ~nb_max:2 tache [serveur1; serveur2] in
  let exp = [[(3, 1., 42); (1, 0.25, 11); (2, 0.25, 8)]; serveur2] in
  compare_liste_serveurs "test_2_options" ~res ~exp

let test_sans_surcharge () =
  let tache = (1, 0.5, 42) in
  let serveur1 = [(2, 0.6, 88)] in
  let res = Repartiteur.repartiteur ~nb_max:2 tache [serveur1] in
  let exp = [[(1, 0.5, 42)] ; [(2, 0.6, 88)]] in
  compare_liste_serveurs "test_sans_surcharge" ~res ~exp

let () =
  test_sujet () ;
  test_simple () ;
  test_sans_surcharge ()
