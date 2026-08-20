(ns note-clj.framework)

(def curr-effect (atom nil))

(defn effect [f]
  (let [prev-effect @curr-effect]
    (reset! curr-effect f)
    (try
      (f)
      (finally
        (reset! curr-effect prev-effect)))))

(defn untrack [f]
  (let [prev-effect @curr-effect]
    (reset! curr-effect nil)
    (try
      (f)
      (finally
        (reset! curr-effect prev-effect)))))

(defn signal [init-value]
    (let [subscribers (atom #{})
          value (atom init-value)]
      ;; Returning getter and setter function
      [(fn []
          (when @curr-effect
            ;; Discover effect
            (swap! subscribers conj @curr-effect))
          @value)
        (fn [new-value]
          (reset! value new-value)
          (doseq [f @subscribers]
            (effect f)))]))
