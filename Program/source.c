#include <stdio.h>

struct bear {
	char grade;
	int health;
	double experience;
};

int main()
{
#pragma region 구조체
	// 여러 개의 변수를 하나의 집합으로 구조화한 다음
	// 하나의 객체를 생성하는 것입니다.
	
	// struct bear unit = { 'S',2500,525.75};
	// 
	// printf("grade : %c\n",unit.grade);
	// printf("health : %d\n", unit.health);
	// printf("experience : %.2f\n\n", unit.experience);
	// 
	// // 구조체의 각 멤버는 구조체 선언에서 나타나는 순서대로 초기화되며
	// // 이 순서는 초기화 목록의 값들이 왼쪽에서부터 오른쪽으로 시작됩니다.
	// 
	// unit.grade = 'A';
	// unit.health = 100;
	// unit.experience = 135.75;
	// 
	// printf("grade : %c\n", unit.grade);
	// printf("health : %d\n", unit.health);
	// printf("experience : %.2f\n", unit.experience);

	// 구조체를 선언하기 전에 구조체는 메모리 공간이 생성되지 않으므로,
	// 구조체 내부에 있는 데이터를 초기화할 수 있습니다.
	
#pragma endregion

	return 0;
}
