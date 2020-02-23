#include<fstream>
#include<iostream>
#include<chrono>
#include<vector>
using namespace std;

int** generate_square(int n){
    // auto start = chrono::high_resolution_clock::now();
    
    int** arr = new int*[n];
	for(int i=0; i<n; i++){
		arr[i] = new int[n]();
	}
    return arr;
}

void generate_rand(int** arr, int n){
	for(int i = 0; i < n; i++){
		for(int j=0; j < n; j++){
			arr[i][j] = rand()%n;
		}
	}
    // auto start = chrono::high_resolution_clock::now();
    // auto end = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::microseconds>(end-start);
    // cout<<"generation ends in "<< duration.count() << endl;
}

void print_square(int **arr, int n, ofstream& fout){
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++)
			fout<<arr[i][j]<<',';
		fout<<endl;
	}
}

void print_square_cout(int **arr, int n){
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++)
			cout<<arr[i][j]<<',';
		cout<<endl;
	}
}

int** add_matrix(int **arr1, int **arr2, int size){
    int** result = generate_square(size);
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            result[i][j] = arr1[i][j] + arr2[i][j];
    return result;
}

int** partial_copy(int **arr, int row, int col, int size){
    int **newarr= generate_square(size);
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            newarr[i][j]=arr[row+i][col+j];
        }
    }
    return newarr;
}
int** multiple_strassen(int **arr1, int **arr2, int size){

    int** result = generate_square(size);
    if(size <= 1){
        result[0][0] = arr1[0][0]*arr2[0][0];
    }
    else
    {
        int **arr1_11 = arr1, 
            **arr1_12 = partial_copy(arr1,0,size/2,size/2),
            **arr1_21 = partial_copy(arr1,size/2,0,size/2),
            **arr1_22 = partial_copy(arr1,size/2,size/2,size/2),
            **arr2_11 = arr2,
            **arr2_12 = partial_copy(arr2,0,size/2,size/2),
            **arr2_21 = partial_copy(arr2,size/2,0,size/2),
            **arr2_22 = partial_copy(arr2,size/2,size/2,size/2);
            
        print_square_cout(arr1_11,size/2);
        print_square_cout(arr1_12,size/2);
        print_square_cout(arr1_21,size/2);
        print_square_cout(arr1_22,size/2);

        print_square_cout(arr2_11,size/2);
        print_square_cout(arr2_12,size/2);
        print_square_cout(arr2_21,size/2);
        print_square_cout(arr2_22,size/2);
        
        
        int** res11 = add_matrix(multiple_strassen(arr1_11,arr2_11,size/2), multiple_strassen(arr1_12,arr2_21,size/2),size/2);
        int** res12 = add_matrix(multiple_strassen(arr1_11,arr2_12,size/2), multiple_strassen(arr1_12,arr2_22,size/2),size/2);
        int** res21 = add_matrix(multiple_strassen(arr1_21,arr2_11,size/2), multiple_strassen(arr1_22,arr2_21,size/2),size/2);
        int** res22 = add_matrix(multiple_strassen(arr1_21,arr2_12,size/2), multiple_strassen(arr1_22,arr2_22,size/2),size/2);


        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                result[i][j] = (i<size/2)?((j<size/2)?res11[i][j]:res12[i][j-size/2]):((j<size/2)?res21[i-size/2][j]:res22[i-size/2][j-size/2]);
            }
        }
    }

    return result;
}

int main(){
    ofstream fout("matrix_result.txt");
    int N=4;

    int **arr1= generate_square(N), **arr2=generate_square(N);
    generate_rand(arr1, N);
    generate_rand(arr2, N);
    
    fout<<"============= arrA ================="<<endl;
	print_square(arr1,N, fout);
    fout<<"===================================="<<endl;
    fout<<"============= arrB ================="<<endl;
    print_square(arr2,N, fout);
    fout<<"===================================="<<endl;
    int **res = multiple_strassen(arr1,arr2,N);

    fout<<"result: "<<endl;
    print_square(res,N,fout);
    fout.close();
}
	