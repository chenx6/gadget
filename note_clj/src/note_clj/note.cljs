(ns note-clj.note
  (:require [note-clj.framework :refer [signal]]
            [note-clj.dom :refer [hiccup->dom]]))

(defrecord NoteInfo [^string id
                     ^string type
                     ^"string[]" children
                     ^string parent])

(defrecord HeaderNote [note-info content])

(defn create-header-note
  ([content]
   (->HeaderNote
    (->NoteInfo (str (random-uuid)) "header" [] nil)
    (signal content)))
  ([info content]
   (->HeaderNote
    (->NoteInfo (:id info) "header" [] nil)
    (signal content))))

(defn on-keydown! [event]
  (when (and (= (.-key event) "Enter") (not (.-isComposing event)))
    (.preventDefault event)
    (.blur (.-target event))))

(defn mount-header-note! [container header on-delete!]
  (let [[content set-content!] (:content header)
        note-id (:id (:note-info header))
        root (hiccup->dom
              [:div {:id note-id
                     :class "note note-header"}
               [:div {:class "dragger" :draggable "true"} "⋮⋮"]
               [:h2 {:class "note-content note-header-content"
                     :contenteditable "plaintext-only"
                     :inner-text content
                     :on-input (fn [event] (set-content! (.-innerText (.-currentTarget event))))
                     :on-keydown on-keydown!}]
               [:button {:class "note-delete" :on-click (fn [_] (on-delete! note-id))} "x"]])]
    (.append container root)
    root))

(defrecord ParagraphNote [note-info content])

(defn create-paragraph-note
  ([content]
   (->ParagraphNote
    (->NoteInfo (str (random-uuid)) "paragraph" [] nil)
    (signal content)))
  ([info content]
   (->ParagraphNote
    (->NoteInfo (:id info) "paragraph" [] nil)
    (signal content))))

(defn mount-paragraph-note! [container prg on-delete!]
  (let [[content set-content!] (:content prg)
        note-id (:id (:note-info prg))
        root (hiccup->dom
               [:div {:id note-id
                      :class "note note-paragraph"}
                [:div {:class "dragger" :draggable "true"} "⋮⋮"]
                [:p {:class "note-content"
                     :contenteditable "plaintext-only"
                     :inner-text content
                     :on-input (fn [event] (set-content! (.-innerText (.-currentTarget event))))}]
                [:button {:class "note-delete" :on-click (fn [_] (on-delete! note-id))} "x"]])]
    (.append container root)
    root))

(defrecord TodoNote [note-info content checked])

(defn create-todo-note
  ([content checked]
   (->TodoNote
    (->NoteInfo (str (random-uuid)) "todo" [] nil)
    (signal content)
    (signal checked)))
  ([info content checked]
   (->TodoNote
    (->NoteInfo (:id info) "todo" [] nil)
    (signal content)
    (signal checked))))

(defn mount-todo-note! [container todo on-delete!]
  (let [[content set-content!] (:content todo)
        [checked set-checked!] (:checked todo)
        note-id (:id (:note-info todo))
        root (hiccup->dom
               [:div {:id note-id
                      :class (fn [] (str "note note-todo" (when (checked) " note-completed")))}
                [:div {:class "dragger" :draggable "true"} "⋮⋮"]
                [:input {:class "note-checkbox"
                         :type "checkbox"
                         :checked checked
                         :on-change (fn [event] (set-checked! (.. event -target -checked)))}]
                [:span {:class "note-content"
                        :contenteditable "plaintext-only"
                        :inner-text content
                        :on-input (fn [event] (set-content! (.-innerText (.-currentTarget event))))
                        :on-keydown on-keydown!}]
                [:button {:class "note-delete" :on-click (fn [_] (on-delete! note-id))} "x"]])]
    (.append container root)
    root))
