val pp_tache : Format.formatter -> 'a Repartiteur.tache -> unit
val pp_liste :
  (Format.formatter -> 'a -> unit) -> Format.formatter -> 'a list -> unit
val pp_serveur : Format.formatter -> 'a Repartiteur.serveur -> unit
val pp_liste_serveurs :
  Format.formatter -> 'a Repartiteur.etat_serveurs -> unit
val pp_charges : Format.formatter -> float list -> unit
val compare_liste_serveurs :
  string ->
  exp:'a Repartiteur.etat_serveurs ->
  res:'a Repartiteur.etat_serveurs -> unit
val compare_surcharge : string -> res:float -> exp:float -> unit
