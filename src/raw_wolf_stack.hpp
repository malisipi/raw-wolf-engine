namespace raw_wolf::stack {
    int get_count_objects(void** items, int stack_len){
        int count = 0;
        for(int index=0; index < stack_len; index++){
            if(items[index] != NULL){
                count++;
            };
        };
        return count;
    }

    int get_empty_index(void** items, int stack_len){
        for(int index=0; index < stack_len; index++){
            if(items[index] == NULL){
                return index;
            };
        };
        return -1;
    }
}
