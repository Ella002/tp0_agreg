let pp_tache fmt ((d, c, _) : 'a Repartiteur.tache) =
  Format.fprintf fmt "(%d, %f, _)" d c

let rec pp_liste pp_elt fmt (s : 'a list) =
    match s with
    | [] -> ()
    | [x] -> Format.fprintf fmt "%a" pp_elt x
    | hd::tl -> Format.fprintf fmt "%a ;@ " pp_elt hd ; pp_liste pp_elt fmt tl

let pp_serveur fmt (serveur : 'a Repartiteur.serveur) =
  Format.fprintf fmt "[ @[<hv>%a@] ]" (pp_liste pp_tache) serveur

let pp_liste_serveurs fmt (etat_serveurs : 'a Repartiteur.etat_serveurs) =
  Format.fprintf fmt "[ @[<hov>%a@] ]" (pp_liste pp_serveur) etat_serveurs

let pp_charges fmt (charges : float list) =
  let pp_charge fmt c = Format.fprintf fmt "%f" c in
  Format.fprintf fmt "[ @[<hv>%a@] ]" (pp_liste pp_charge) charges

let compare_liste_serveurs (name : string) ~(exp : 'a Repartiteur.etat_serveurs) ~(res : 'a Repartiteur.etat_serveurs) =
  let compare_taches acc (d1, c1, x1) (d2, c2, x2) =
    let res = acc && d1 = d2 && c1 = c2 in
    if not res then
      begin
        Format.(fprintf std_formatter "Difference between %a and %a@."
                  pp_tache (d1, c1, x1) pp_tache (d2, c2, x2)) ;
        Format.(fprintf std_formatter "acc: %B, cmp1: %B, cmp2: %B@."
                  acc (d1 = d2) (c1 = c2))
      end ;
    res
  in
  let compare_serveurs acc s1 s2 =
    if List.length s1 != List.length s2
    then false
    else List.fold_left2 compare_taches acc s1 s2
  in
  let flag =
    if List.length exp != List.length res
    then false
    else List.fold_left2 compare_serveurs true exp res
  in
  if not flag then
    Format.(fprintf
              std_formatter
              "@[<v>Problem with %s.@ Expected:@ %a.@ Result:@ %a.@]@."
              name
              pp_liste_serveurs exp
              pp_liste_serveurs res)
  else
    Format.printf "%s worked@." name

let compare_surcharge (name : string) ~(res : float) ~(exp : float) =
  let flag = (res -. exp) < 0.0001 in
  if not flag then
    Format.(fprintf std_formatter "Problem with %s. Expected: %f. Result: %f.@."
              name
              exp
              res)
  else
    Format.printf "%s worked@." name
