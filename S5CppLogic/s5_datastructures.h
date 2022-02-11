#pragma once
#include "s5data.h"

#include <vector>
#include <list>

namespace shok {

	template<class T>
	struct TreeNode {
		TreeNode<T>* left, * parent, * right;
		T data;
		bool redblack;
	};
	template<class T>
	struct Set { // todo iterators
		PADDINGI(1);
		TreeNode<T>* root;
		int size;

	private:
		void ForAllRec(TreeNode<T>* d, std::function<void(T*)> lambda) {
			if (d == root)
				return;
			ForAllRec(d->left, lambda);
			lambda(&d->data);
			ForAllRec(d->right, lambda);
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
	};


	struct String {
	private:
		int u1 = 0;
		int data;
		int u[4] = { 0 };
		int size = 0;

	public:
		String(const char* s);
		String(const String& c);
		void assign(const char* s);
		const char* c_str();
		~String();
		String();
	};
	static_assert(sizeof(String) == 7 * 4);

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
		auto operator[](size_t p) {
			return Internal[p];
		}
		const auto operator[](size_t p) const {
			return Internal[p];
		}
		size_t size() const noexcept {
			return Internal.size();
		}
		const T* data() const noexcept {
			return Internal.data();
		}

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
}
