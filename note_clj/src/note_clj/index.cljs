(ns note-clj.index
  (:require [note-clj.framework :refer [signal effect untrack]]
            [note-clj.note :refer [create-todo-note mount-todo-note!
                                   create-paragraph-note mount-paragraph-note!
                                   create-header-note mount-header-note!]]
            [note-clj.drag :refer [get-drop-position move-to-index]]
            [note-clj.dom :refer [hiccup->dom]]
            [note-clj.store :refer [load-notes store-notes!]]
            [note-clj.command :refer [mount-command!]]))

(def notes-state
  (signal
   (or (not-empty (load-notes))
       [(create-header-note "标题")
        (create-paragraph-note "这是文本")
        (create-todo-note "学习" false)
        (create-todo-note "写项目" true)])))
(def notes (first notes-state))
(def set-notes! (second notes-state))

(defn remove-note-by-id [notes id]
  (filterv #(not= id (:id (:note-info %))) notes))

(defn note-id [note]
  (:id (:note-info note)))

(defn delete-note! [id]
  (set-notes! (remove-note-by-id (notes) id)))

(defn focus-note! [note-id]
  (when-let [note-node (.getElementById js/document note-id)]
    (when-let [content-node (.querySelector note-node ".note-content")]
      (.focus content-node)
      (let [selection (.getSelection js/window)]
        (.selectAllChildren selection content-node)
        (.collapseToEnd selection)))))

(defn add-note! [note]
  (set-notes! (conj (notes) note))
  (focus-note! (note-id note)))

(defn mount-note! [notes-container note]
  (let [note-type (:type (:note-info note))]
    (cond
      (= note-type "todo")
      (mount-todo-note! notes-container note delete-note!)
      (= note-type "paragraph")
      (mount-paragraph-note! notes-container note delete-note!)
      (= note-type "header")
      (mount-header-note! notes-container note delete-note!))))

(defn sync-notes! [notes-container state-notes]
  (let [state-ids (set (map note-id state-notes))]
    ;; Remove DOM notes that are no longer in state.
    (doseq [el (.querySelectorAll notes-container ".note")]
      (when-not (contains? state-ids (.-id el))
        (.remove el)))
    ;; Mount state notes that are missing from the DOM.
    (let [current-ids (set (map #(.-id %) (.querySelectorAll notes-container ".note")))]
      (doseq [note state-notes]
        (when-not (contains? current-ids (note-id note))
          (mount-note! notes-container note))))
    ;; Reorder existing/mounted elements to match state order.
    (doseq [note state-notes]
      (when-let [el (.getElementById js/document (note-id note))]
        (.appendChild notes-container el)))))

(defn mount-app! []
  (let [dragging-note (atom nil)
        drop-index (atom nil)
        drop-line (hiccup->dom [:div {:class "drop-line"}])
        clean-drag! (fn []
                      (when-let [note @dragging-note]
                        (.remove (.-classList note) "dragging"))
                      (.remove drop-line)
                      (reset! dragging-note nil)
                      (reset! drop-index nil))
        root (hiccup->dom
              [:div {:class "app-content"}
               [:div
                {:id "notes"
                 :on-dragstart (fn [event]
                                 (let [target (.-target event)
                                       parent-node (.-parentNode target)]
                                   (when (and (.contains (.-classList target) "dragger") (not= parent-node nil))
                                     (reset! dragging-note parent-node)
                                     (set! (-> event .-dataTransfer .-effectAllowed) "move")
                                     (.setData (.-dataTransfer event) "text/plain" (.-id @dragging-note)))))
                 :on-dragover (fn [event]
                                (.preventDefault event)
                                (when (not= @dragging-note nil)
                                  (set! (-> event .-dataTransfer .-dropEffect) "move")
                                  (let [notes-container (.-currentTarget event)
                                        [index before-elem] (get-drop-position notes-container
                                                                               @dragging-note
                                                                               (.-clientY event))]
                                    (reset! drop-index index)
                                    ;; Insert drop-line into container
                                    (if (= before-elem nil)
                                      (.append notes-container drop-line)
                                      (.insertBefore notes-container drop-line before-elem)))))
                 :on-drop (fn [event]
                            (.preventDefault event)
                            (when (and @dragging-note (some? @drop-index))
                              (let [moving-id (.-id @dragging-note)]
                                ;; Move note done, recalculate position
                                (set-notes! (move-to-index (notes) moving-id @drop-index))))
                            (clean-drag!))
                 :on-dragend (fn [_]
                               (clean-drag!))}]])
        notes-container (.querySelector root "#notes")
        app (.querySelector js/document "#app")]
    (mount-command! root add-note!)
    (.append app root)
    (effect
     (fn []
       (let [state-notes (notes)]
         (untrack #(sync-notes! notes-container state-notes)))))
    (effect (fn [] (store-notes! (notes))))))

(mount-app!)
