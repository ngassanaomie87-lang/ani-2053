# Comparaison : Git Merge vs Git Rebase

## 1. Visualisation des graphes
Après avoir réalisé les deux intégrations sur mon dépôt d'essai, j'ai affiché l'historique des commits avec la commande `git log --oneline --graph --all`.

* **Le graphe avec `git merge` (Fusion classique) :**
  Il présente une structure en arborescence. On y voit clairement les embranchements et un "commit de fusion" (merge commit) final qui relie la branche de travail à la branche principale (`main`). L'historique conserve la chronologie exacte et la réalité des modifications parallèles.

* **Le graphe avec `git rebase` (Rejeu) :**
  Il présente une structure totalement linéaire. Tous les commits de la branche de travail ont été replacés à la suite du dernier commit de la branche `main`, comme s'ils avaient été écrits directement sur le tronc principal.

## 2. Préférence et Argumentation
**Mon choix :** Je préfère lire le graphe obtenu par **`git rebase`**.

**Argument :** 
Bien que le `merge` soit plus transparent sur l'existence historique de branches parallèles, le `rebase` offre un historique linéaire beaucoup plus propre, fluide et facile à lire. Il évite l'enchevêtrement des lignes de fusion (les fameux "pattes d'araignée") et simplifie grandement la relecture du code ou la recherche d'un bug via `git bisect`.