#include <stdio.h>
#include <stdlib.h>

void main()
{
#pragma region 동적 할당
	// 프로그램을 실행하는 중에 필요한 만큼 메모리를 
	// 할당하는 작업입니다.

	// int* memory = malloc(sizeof(int));
	// *memory = 99;
	// printf("memory 변수가 가리키는 값 : %d\n", *memory);
	// free(memory);

	// 동적 할당은 실행 시간에 가변적으로 메모리의 크기를
	// 변경시킬 수 있으며, 동적으로 메모리의 크기를 할당할 때
	// 바이트 단위로 지정합니다.

	// memory = malloc(sizeof(int) * 3);
	// for (int i = 0; i < 3; i++) {
	// 	memory[i] = (i + 1) * 10;
	//		printf("memory[%d] : %d\n", i, memory[i]);
	// }
	// free(memory);

	// 메모리를 동적으로 할당한 상태에서 해제하지 않으면
	// 메모리 누수 현상이 일어나므로, 메모리 누수가 일어나게 
	// 되면 더 이상 메모리를 할당할 수 없습니다.

#pragma endregion
}
