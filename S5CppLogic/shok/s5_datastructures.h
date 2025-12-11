#pragma once
#include <compare>
#include <vector>
#include <list>
#include <set>
#include <deque>
#include <array>
#include <functional>
#include <type_traits>
#include <ranges>

#include <shok/s5_forwardDecls.h>
#include <shok/s5_mem.h>

namespace shok {
	template<class T>
	const T& Tree_DefaultExtractKey(const T& t) {
		return t;
	}
	template<class T>
	requires std::three_way_comparable<T>
	std::strong_ordering Tree_DefaultComparator(const T& l, const T& r) {
		return std::compare_strong_order_fallback(l, r);
	}

	template<class T, class K
		, const K&(*KeyExtractor)(const T&) = Tree_DefaultExtractKey
		, std::strong_ordering (*Comparator)(const K&, const K&)=Tree_DefaultComparator
		, bool Multi = false>
	class Tree {
	private:
		enum class Color : byte {
			Red = 0,
			Black = 1,
		};

		struct TreeNode {
			TreeNode* left = nullptr;
			TreeNode* parent = nullptr;
			TreeNode* right = nullptr;
			T data;
			Color c = Color::Red;
			bool isnil = false;


			void* operator new(size_t s) {
				return shok::Malloc(s);
			}
			void operator delete(void* p) {
				shok::Free(p);
			}

			template<class ... Args>
			TreeNode(Args&& ... args) : data(std::forward<Args>(args)...) {
			}
		};
		PADDINGI(1);
		TreeNode* root;
	public:
		int size = 0;

	private:
		void ForAllRec(TreeNode* d, std::function<void(T*)> lambda) {
			if (d == root)
				return;
			ForAllRec(d->left, lambda);
			lambda(&d->data);
			ForAllRec(d->right, lambda);
		}
		TreeNode* FollowDownLeft(TreeNode* node) const {
			if (!node)
				return nullptr;
			while (node->left != root) {
				node = node->left;
			}
			return node;
		}
		TreeNode* FollowDownRight(TreeNode* node) const {
			if (!node)
				return nullptr;
			while (node->right != root) {
				node = node->right;
			}
			return node;
		}
		TreeNode* NextNodeIteration(TreeNode* node) const {
			if (node->right != root) { // first check right, then go down left as far as possible
				node = node->right;
				return FollowDownLeft(node);
			}
			else { // go back up until we hit root or did get there by going left
				TreeNode* p = node->parent;
				while (p != root && node == p->right) {
					node = p;
					p = p->parent;
				}
				if (p == root)
					return nullptr;
				else
					return p;
			}
		}

	public:
		void ForAll(std::function<void(T*)> lambda) {
			ForAllRec(root->parent, lambda);
		}
		T* GetFirstMatch(std::function<bool(T*)> lambda) {
			T* r = nullptr;
			ForAll([&r, lambda](T* d) {
				if (r == nullptr && lambda(d))
					r = d;
				});
			return r;
		}

		class Iter {
			friend class Tree;
			TreeNode* node;
			const Tree<T, K, KeyExtractor, Comparator, Multi>* set;
			Iter(TreeNode* n, const Tree<T, K, KeyExtractor, Comparator, Multi>* s) {
				node = n;
				set = s;
			}

		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			constexpr Iter() : node(nullptr), set(nullptr) {}

			T& operator*() const {
				return node->data;
			}
			T* operator->() const {
				return &node->data;
			}
			bool operator==(const Iter& o) const {
				return this->node == o.node;
			}
			Iter& operator++() {
				node = set->NextNodeIteration(node);
				return *this;
			}
			Iter operator++(int) {
				Iter r = *this;
				++(*this);
				return r;
			}
		};
		friend class Tree<T, K, KeyExtractor, Comparator, Multi>::Iter;

		Iter begin() const {
			if (root->left == root)
				return end();
			return { root->left, this };
		}
		Iter end() const {
			return { nullptr, this };
		}

		static std::strong_ordering Compare(const T& l, const T& r) {
			return Comparator(KeyExtractor(l), KeyExtractor(r));
		}
		static std::strong_ordering CompareKV(const K& l, const T& r) {
			return Comparator(l, KeyExtractor(r));
		}
		static std::strong_ordering CompareVK(const T& l, const K& r) {
			return Comparator(KeyExtractor(l), r);
		}

	private:
		void RotateLeft(TreeNode* n) {
			TreeNode* pn = n->right;
			n->right = pn->left;

			if (!pn->left->isnil) {
				pn->left->parent = n;
			}

			pn->parent = n->parent;

			if (n == root->parent) {
				root->parent = pn;
			}
			else if (n == n->parent->left) {
				n->parent->left = pn;
			}
			else {
				n->parent->right = pn;
			}

			pn->left = n;
			n->parent = pn;
		}
		void RotateRight(TreeNode* n) {
			TreeNode* pn = n->left;
			n->left = pn->right;

			if (!pn->right->isnil) {
				pn->right->parent = n;
			}

			pn->parent = n->parent;

			if (n == root->parent) {
				root->parent = pn;
			}
			else if (n == n->parent->right) {
				n->parent->right = pn;
			}
			else {
				n->parent->left = pn;
			}

			pn->right = n;
			n->parent = pn;
		}

		struct InsertInfo {
			TreeNode* parent = nullptr;
			TreeNode* bound = nullptr;
			bool asLeft = false;
		};

		TreeNode* InsertNode(const InsertInfo& i, TreeNode* newnode) {
			++size;
			newnode->parent = i.parent;
			if (i.parent == root) { // first in tree
				root->left = newnode;
				root->parent = newnode;
				root->right = newnode;
				newnode->c = Color::Black;
				return newnode;
			}

			if (!i.asLeft) {
				i.parent->right = newnode;
				if (i.parent == root->right)
					root->right = newnode;
			}
			else {
				i.parent->left = newnode;
				if (i.parent == root->left)
					root->left = newnode;
			}

			for (TreeNode* pn = newnode; pn->parent->c == Color::Red;) {
				if (pn->parent == pn->parent->parent->left) {
					TreeNode* ppr = pn->parent->parent->right;
					if (ppr->c == Color::Red) {
						pn->parent->c = Color::Black;
						ppr->c = Color::Black;
						pn->parent->parent->c = Color::Red;
						pn = pn->parent->parent;
					}
					else {
						if (pn == pn->parent->right) {
							pn = pn->parent;
							RotateLeft(pn);
						}
						pn->parent->c = Color::Black;
						pn->parent->parent->c = Color::Red;
						RotateRight(pn->parent->parent);
					}
				}
				else {
					TreeNode* ppl = pn->parent->parent->left;
					if (ppl->c == Color::Red) {
						pn->parent->c = Color::Black;
						ppl->c = Color::Black;
						pn->parent->parent->c = Color::Red;
						pn = pn->parent->parent;
					}
					else {
						if (pn == pn->parent->left) {
							pn = pn->parent;
							RotateRight(pn);
						}
						pn->parent->c = Color::Black;
						pn->parent->parent->c = Color::Red;
						RotateLeft(pn->parent->parent);
					}
				}
			}

			root->parent->c = Color::Black;
			return newnode;
		}

		InsertInfo FindUpperBound(const K& key) {
			InsertInfo r{ root->parent, root, false };
			TreeNode* tn = root->parent;
			while (!tn->isnil) {
				r.parent = tn;
				if (CompareKV(key, tn->data) == std::strong_ordering::less) {
					r.asLeft = true;
					r.bound = tn;
					tn = tn->left;
				}
				else {
					r.asLeft = false;
					tn = tn->right;
				}
			}
			return r;
		}

		InsertInfo FindLowerBound(const K& key) {
			InsertInfo r{ root->parent, root, false };
			TreeNode* tn = root->parent;
			while (!tn->isnil) {
				r.parent = tn;
				if (CompareVK(tn->data, key) == std::strong_ordering::less) {
					r.asLeft = false;
					tn = tn->right;
				}
				else {
					r.asLeft = true;
					r.bound = tn;
					tn = tn->left;
				}
			}
			return r;
		}
		bool LowerBoundDuplicate(TreeNode* b, const K& key) {
			return !b->isnil && !(CompareKV(key, b->data) == std::strong_ordering::less);
		}

		template<class ... Args>
		std::pair<TreeNode*, bool> Emplace(Args&& ... args) {
			TreeNode* toinsert = new TreeNode(std::forward<Args>(args)...);
			toinsert->right = root;
			toinsert->parent = root;
			toinsert->left = root;
			const K& key = KeyExtractor(toinsert->data);
			if constexpr (Multi) {
				InsertInfo i = FindUpperBound(key);
				return { InsertNode(i, toinsert), true };
			}
			else {
				InsertInfo i = FindLowerBound(key);
				if (LowerBoundDuplicate(i.bound, key)) {
					delete toinsert;
					return { i.bound, false };
				}
				return { InsertNode(i, toinsert), true };
			}
		}

		void Extract(TreeNode* ex) {
			TreeNode* fixnode = nullptr;
			TreeNode* fixnodeparent = nullptr;
			TreeNode* pn = ex;
			
			if (pn->left->isnil) {
				fixnode = pn->right;
			}
			else if (pn->right->isnil) {
				fixnode = pn->left;
			}
			else {
				pn = NextNodeIteration(pn);
				fixnode = pn->right;
			}

			if (pn == ex) {
				fixnodeparent = ex->parent;
				if (!fixnode->isnil)
					fixnode->parent = fixnodeparent;

				if (root->parent == ex) {
					root->parent = fixnode;
				}
				else if (fixnodeparent->left == ex) {
					fixnodeparent->left = fixnode;
				}
				else {
					fixnodeparent->right = fixnode;
				}

				if (root->left == ex) {
					root->left = fixnode->isnil ? fixnodeparent : FollowDownLeft(fixnode);
				}

				if (root->right == ex) {
					root->right = fixnode->isnil ? fixnodeparent : FollowDownRight(fixnode);
				}
			}
			else {
				ex->left->parent = pn;
				pn->left = ex->left;

				if (pn == ex->right) {
					fixnodeparent = pn;
				}
				else {
					fixnodeparent = pn->parent;
					if (!fixnode->isnil) {
						fixnode->parent = fixnodeparent;
					}

					fixnodeparent->left = fixnode;
					pn->right = ex->right;
					ex->right->parent = pn;
				}

				if (root->parent == ex) {
					root->parent = pn;
				}
				else if (ex->parent->left == ex) {
					ex->parent->left = pn;
				}
				else {
					ex->parent->right = pn;
				}

				pn->parent = ex->parent;
				std::swap(pn->c, ex->c);
			}

			if (ex->c == Color::Black) {
				for (; fixnode != root->parent && fixnode->c == Color::Black; fixnodeparent = fixnode->parent) {
					if (fixnode == fixnodeparent->left) {
						pn = fixnodeparent->right;
						if (pn->c == Color::Red) {
							pn->c = Color::Black;
							fixnodeparent->c = Color::Red;
							RotateLeft(fixnodeparent);
							pn = fixnodeparent->right;
						}

						if (pn->isnil) {
							fixnode = fixnodeparent;
						}
						else if (pn->left->c == Color::Black && pn->right->c == Color::Black) {
							pn->c = Color::Red;
							fixnode = fixnodeparent;
						}
						else {
							if (pn->right->c == Color::Black) {
								pn->left->c = Color::Black;
								pn->c = Color::Red;
								RotateRight(pn);
								pn = fixnodeparent->right;
							}
							pn->c = fixnodeparent->c;
							fixnodeparent->c = Color::Black;
							pn->right->c = Color::Black;
							RotateLeft(fixnodeparent);
							break;
						}
					}
					else {
						pn = fixnodeparent->left;
						if (pn->c == Color::Red) {
							pn->c = Color::Black;
							fixnodeparent->c = Color::Red;
							RotateRight(fixnodeparent);
							pn = fixnodeparent->left;
						}

						if (pn->isnil) {
							fixnode = fixnodeparent;
						}
						else if (pn->right->c == Color::Black && pn->left->c == Color::Black) {
							pn->c = Color::Red;
							fixnode = fixnodeparent;
						}
						else {
							if (pn->left->c == Color::Black) {
								pn->right->c = Color::Black;
								pn->c = Color::Red;
								RotateLeft(pn);
								pn = fixnodeparent->left;
							}

							pn->c = fixnodeparent->c;
							fixnodeparent->c = Color::Black;
							pn->left->c = Color::Black;
							RotateRight(fixnodeparent);
							break;
						}
					}
				}

				fixnode->c = Color::Black;
			}

			if (size > 0)
				--size;
		}

		TreeNode* SearchRec(const K& c, TreeNode* n) {
			if (n == root)
				return nullptr;
			std::strong_ordering cmp = CompareKV(c, n->data);
			if (cmp == std::strong_ordering::equal)
				return n;
			if (cmp == std::strong_ordering::less)
				return SearchRec(c, n->left);
			else
				return SearchRec(c, n->right);
		}
	protected:
		TreeNode* Search(const K& c) {
			return SearchRec(c, root->parent);
		}
	public:
		Iter insert(T&& d) {
			return { Emplace(std::forward<T>(d)).first, this };
		}
		template<class ... Args>
		Iter emplace(Args&& ... args) {
			return { Emplace(std::forward<Args>(args)...).first, this };
		}
		Iter erase(Iter wh) {
			Iter next = wh;
			++next;
			TreeNode* toex = wh.node;
			Extract(toex);
			delete toex;
			return next;
		}
		Iter erase(Iter beg, Iter end) {
			while (beg != end) {
				beg = erase(beg);
			}
			return beg;
		}
		// could be more efficent, but it works...
		void clear() {
			Iter i = begin();
			while (i != end())
				i = erase(i);
		}
		Iter find(const K& key) {
			return { Search(key), this };
		}
		Iter lower_bound(const K& key) {
			InsertInfo i = FindLowerBound(key);
			return { i.bound == root ? nullptr : i.bound, this };
		}
		Iter upper_bound(const K& key) {
			InsertInfo i = FindUpperBound(key);
			return { i.bound == root ? nullptr : i.bound, this };
		}

		Tree() {
			// not call ctor, to not call data ctor, root does not have actual data
			root = static_cast<TreeNode*>(shok::Malloc(sizeof(TreeNode)));
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnontrivial-memcall"
			std::memset(root, 0, sizeof(TreeNode));
#pragma clang diagnostic pop
			root->isnil = true;
			root->c = Color::Black;
			root->right = root;
			root->parent = root;
			root->left = root;
		}
		~Tree() {
			clear();
			// not call dtor, to not call data dtor, root does not have actual data
			shok::Free(root);
			root = nullptr;
		}
	};
	static_assert(sizeof(Tree<int, int>) == 3 * 4);

	template<class T>
	requires std::three_way_comparable<T>
	class Set : public Tree<T, T, Tree_DefaultExtractKey, Tree_DefaultComparator, false> {
		
	};
	static_assert(sizeof(Set<int>) == 3 * 4);

	template<class K, class V>
	const K& Map_DefaultExtractKey(const std::pair<K, V>& d) {
		return d.first;
	}

	template<class K, class V, std::strong_ordering(*Comparator)(const K&, const K&)=Tree_DefaultComparator>
	requires std::three_way_comparable<K>
	class Map : public Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, false> {

	public:
		V& at(const K& key) {
			auto d = Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, false>::find(key);
			if (d == Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, false>::end())
				throw std::out_of_range{ "key not in map" };
			return d->second;
		}
		void insert(K&& key, V&& val) {
			std::pair<K, V> i{ key, val };
			Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, false>::insert(std::move(i));
		}
	};
	static_assert(sizeof(Map<int, int>) == 3 * 4);

	template<class K, class V, std::strong_ordering(*Comparator)(const K&, const K&) = Tree_DefaultComparator>
	requires std::three_way_comparable<K>
	class MultiMap : public Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, true> {
	public:
		void insert(K&& key, V&& val) {
			std::pair<K, V> i{ key, val };
			Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, true>::insert(std::move(i));
		}

		auto ForKeys(const K& k)
		{
			using iter = Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, true>::Iter;
			iter be = Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, true>::lower_bound(k);
			iter en = Tree<std::pair<K, V>, K, Map_DefaultExtractKey, Comparator, true>::upper_bound(k);
			return std::ranges::subrange<iter, iter, std::ranges::subrange_kind::unsized>(be, en);
		}
	};
	static_assert(sizeof(MultiMap<int, int>) == 3 * 4);


	struct String {
	private:
		int u1 = 0;
		union {
			char* alloc;
			char inlin[4 * 4] = {};
		} data;
		size_t size_v = 0;
		size_t allocated = 0;

	public:
		explicit String(const char* s);
		explicit String(const String& c);
		explicit String(const std::string& s);
		explicit String(const std::string_view& s);
		void assign(const char* s);
		void assign(const char* s, size_t len);
		String& append(String& toappend, size_t pos, size_t count = std::string::npos);
		String& append(String& toappend);
		void resize(size_t len, char c = '\0');
		const char* c_str() const;
		size_t size() const;
		~String();
		String();
		std::strong_ordering operator<=>(const String& r) const;
		bool operator==(const String& r) const;
		void operator=(const String& s);
		void operator=(const std::string& s);
		void operator=(const std::string_view& s);
		void operator=(const char* s);
		operator std::string_view() const;
	};
	static_assert(sizeof(String) == 7 * 4);
	std::strong_ordering operator<=>(const String& a, std::string_view b);
	bool operator==(const String& a, std::string_view b);
	std::strong_ordering operator<=>(std::string_view a, const String& b);
	bool operator==(std::string_view a, const String& b);

	template<class T>
	struct Vector {
#ifndef _DEBUG
		PADDINGI(1);
#endif
	private:
		std::vector<T, Allocator<T>> Internal;

	public:
		auto begin() noexcept {
			return Internal.begin();
		}
		const auto begin() const noexcept {
			return Internal.begin();
		}
		auto end() noexcept {
			return Internal.end();
		}
		const auto end() const noexcept {
			return Internal.end();
		}
		// these get used by vs for some reason over the official begin/end in for loops
		auto _Unchecked_begin() noexcept {
			return Internal._Unchecked_begin();
		}
		auto _Unchecked_begin() const noexcept {
			return Internal._Unchecked_begin();
		}
		auto _Unchecked_end() noexcept {
			return Internal._Unchecked_end();
		}
		auto _Unchecked_end() const noexcept {
			return Internal._Unchecked_end();
		}
		auto& operator[](size_t p) {
			return Internal[p];
		}
		const auto& operator[](size_t p) const {
			return Internal[p];
		}
		size_t size() const noexcept {
			return Internal.size();
		}
		const T* data() const noexcept {
			return Internal.data();
		}
		T* data() noexcept {
			return Internal.data();
		}
		T& at(size_t p) {
			return Internal.at(p);
		}
		const T& at(size_t p) const {
			return Internal.at(p);
		}

#ifdef _DEBUG
		Vector() {
			// hacky way of getting rid of the std::vectors debug stuff, initialize the vector with 0 (as its done in shok)
			byte a[sizeof(Vector)];
			std::memcpy(a, this, sizeof(Vector));
			std::memset(this, 0, sizeof(Vector));
			reinterpret_cast<Vector*>(a)->~Vector();
		}
		Vector(Vector&& other) noexcept {
			/// hacky move
			std::memcpy(this, &other, sizeof(Vector));
			std::memset(&other, 0, sizeof(Vector));
		}
		Vector(const Vector& other) : Vector() {
			auto th = SaveVector();
			th.Vector = other.Internal;
		}
		Vector& operator=(const Vector& other) {
			auto th = SaveVector();
			th.Vector = other.Internal;
			return *this;
		}
		Vector& operator=(Vector&& other) noexcept {
			/// hacky move
			std::memcpy(this, &other, sizeof(Vector));
			std::memset(&other, 0, sizeof(Vector));
			return *this;
		}
		~Vector() noexcept {
			*reinterpret_cast<int*>(this) = 0;
		}
#endif

		struct SaveVector_Data {
#ifdef _DEBUG
			std::vector<T, Allocator<T>> Vector{};
		private:
			int backu[3] = {};
			std::vector<T, Allocator<T>>& real;

		public:
			SaveVector_Data(std::vector<T, Allocator<T>>& v) : real(v) {
				int* vecPoint = reinterpret_cast<int*>(&real);
				int* savePoint = reinterpret_cast<int*>(&Vector);
				for (int i = 1; i < 4; i++) {
					backu[i - 1] = savePoint[i];
					savePoint[i] = vecPoint[i];
				}
			}
			~SaveVector_Data() {
				int* vecPoint = reinterpret_cast<int*>(&real);
				int* savePoint = reinterpret_cast<int*>(&Vector);
				for (int i = 1; i < 4; i++) {
					vecPoint[i] = savePoint[i];
					savePoint[i] = backu[i - 1];
				}
			}
#else
			std::vector<T, Allocator<T>>& Vector;
#endif
		};

		// use this to make anythink more complicated than iterating over the vector.
		// use always as stack variable!
		SaveVector_Data SaveVector() {
			return SaveVector_Data{ Internal };
		}
	};
	static_assert(sizeof(Vector<int>) == 4 * 4);

	template<class T>
	class List {
#ifndef _DEBUG
		PADDINGI(1);
#endif
	private:
		std::list<T, shok::Allocator<T>> Internal{};

	public:
		auto begin() noexcept {
			return Internal.begin();
		}
		const auto begin() const noexcept {
			return Internal.begin();
		}
		auto end() noexcept {
			return Internal.end();
		}
		const auto end() const noexcept {
			return Internal.end();
		}
		// these get used by vs for some reason over the official begin/end in for loops
		auto _Unchecked_begin() noexcept {
			return Internal._Unchecked_begin();
		}
		auto _Unchecked_begin() const noexcept {
			return Internal._Unchecked_begin();
		}
		auto _Unchecked_end() noexcept {
			return Internal._Unchecked_end();
		}
		auto _Unchecked_end() const noexcept {
			return Internal._Unchecked_end();
		}
		size_t size() const noexcept {
			return Internal.size();
		}

		struct SaveList_Data {
#ifdef _DEBUG
			std::list<T, Allocator<T>> List{};
		private:
			int backu[2] = {};
			std::list<T, Allocator<T>>& real;

		public:
			SaveList_Data(std::list<T, Allocator<T>>& v) : real(v) {
				int* vecPoint = reinterpret_cast<int*>(&real);
				int* savePoint = reinterpret_cast<int*>(&List);
				for (int i = 1; i < 3; i++) {
					backu[i - 1] = savePoint[i];
					savePoint[i] = vecPoint[i];
				}
			}
			~SaveList_Data() {
				int* vecPoint = reinterpret_cast<int*>(&real);
				int* savePoint = reinterpret_cast<int*>(&List);
				for (int i = 1; i < 3; i++) {
					vecPoint[i] = savePoint[i];
					savePoint[i] = backu[i - 1];
				}
			}
#else
			std::list<T, shok::Allocator<T>>& List;
#endif
		};

		// use this to make anythink more complicated than iterating over the list.
		// use always as stack variable!
		SaveList_Data SaveList() {
			return SaveList_Data{ Internal };
		}
	};
	static_assert(sizeof(List<int>) == 3 * 4);

	template<class T>
	struct Deque {
	private:
		std::deque<T, shok::Allocator<T>> Internal{};


	public:
		// standard iterators dont seem to work
	};
	static_assert(sizeof(Deque<int>) == 4 * 5);

	template<class T, size_t S>
	struct Array {
		std::array<T, S> Data = {};
		size_t NextToDeserialize = 0;
	};
	static_assert(sizeof(Array<int, 4>) == 5 * 4);
}
