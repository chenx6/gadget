(ns note-clj.drag)

(defn move-to-index [items moving-id target-index]
  (if-let [moving-item
           (first (filter #(= moving-id (:id (:note-info %))) items))]
    (let [remaining
          (filterv #(not= moving-id (:id (:note-info %))) items)
          target-index
          (-> target-index (max 0) (min (count remaining)))]
      (vec
       (concat
        (subvec remaining 0 target-index)
        [moving-item]
        (subvec remaining target-index))))
    items))

(defn get-drop-position [notes-container dragging-note y]
  (let [candidates
        (filterv
         #(not= % dragging-note)
         (.querySelectorAll notes-container ".note"))

        target-index
        (count
         (take-while
          (fn [note]
            (let [rect (.getBoundingClientRect note)
                  middle (+ (.-top rect)
                            (/ (.-height rect) 2))]
              (> y middle)))
          candidates))]
    [target-index (get candidates target-index)]))