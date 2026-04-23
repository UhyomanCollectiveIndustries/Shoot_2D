#pragma once

template<typename T,int MAX>
class ObjectPool {
public:
	T pool[MAX];

	//フリーリスト
	int freeList[MAX];
	int freeCount;

	//アクティブリスト
	int activeList[MAX];
	int activeIndex[MAX];
	int activeCount_;

	//コンストラクタ
	ObjectPool() {
		freeCount = MAX;
		activeCount_ = 0;

		for (int i = 0; i < MAX; i++){
			freeList[i] = i;
			
			activeList[i] = 0;
			activeIndex[i] = 0;
		}
	}

	//生成
	T* alloc() {
		if (freeCount == 0) return nullptr;

		//フリーリストからインデックスを取得
		int index = freeList[--freeCount];

		//アクティブリストに追加
		activeList[activeCount_] = index;
		activeIndex[index] = activeCount_;
		activeCount_++;

		//注:返されたオブジェクトは呼び出し側で初期化
		return &pool[index];
	}

	//解放
	void free(T* e) {
		if (!e) return;
		
		//インデックスの計算
		int index = e - pool;

		//アクティブリストから削除(フリーリストに戻す)
		freeList[freeCount++] = index;

		//削除位置を取得
		int pos = activeIndex[index];
		int last = activeList[--activeCount_];

		//最後の要素を削除位置に移動
		if (last != index) {
			activeList[pos] = last;
			activeIndex[last] = pos;
		}
	}

	template<typename Func>
	void forEachActive(Func func) {
		for (int i = 0; i < activeCount_; i++) {
			func(pool[activeList[i]]);
		}
	}
};