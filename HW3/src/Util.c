#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Util.h"
#include "Command.h"
#include "Table.h"
#include "SelectState.h"

///
/// Allocate State_t and initialize some attributes
/// Return: ptr of new State_t
///
State_t* new_State() {
    State_t *state = (State_t*)malloc(sizeof(State_t));
    state->saved_stdout = -1;
    return state;
}

///
/// Print shell prompt
///
void print_prompt(State_t *state) {
    if (state->saved_stdout == -1) {
        printf("db > ");
    }
}

///
/// Print the user in the specific format
///
void print_user(User_t *user, SelectArgs_t *sel_args) {
    size_t idx;
    printf("(");
    for (idx = 0; idx < sel_args->fields_len; idx++) {
        if (!strncmp(sel_args->fields[idx], "*", 1)) {
            printf("%d, %s, %s, %d", user->id, user->name, user->email, user->age);
        } else {
            if (idx > 0) printf(", ");

            if (!strncmp(sel_args->fields[idx], "id", 2)) {
                printf("%d", user->id);
            } else if (!strncmp(sel_args->fields[idx], "name", 4)) {
                printf("%s", user->name);
            } else if (!strncmp(sel_args->fields[idx], "email", 5)) {
                printf("%s", user->email);
            } else if (!strncmp(sel_args->fields[idx], "age", 3)) {
                printf("%d", user->age);
            }
        }
    }
    printf(")\n");
}

///
/// Print the users for given offset and limit restriction
///
void print_users(Table_t *table, int *idxList, size_t idxListLen, Command_t *cmd) {
    size_t idx;
    int limit = cmd->cmd_args.sel_args.limit;
    int offset = cmd->cmd_args.sel_args.offset;

    if (offset == -1) {
        offset = 0;
    }

    if (cmd->cmd_args.sel_args.where==1)
    {
	int* where=(int*)malloc(sizeof(int)*table->len);
	int pos=0,i,flag,tmp;	//flag:where1 accepted

	for(i=0;i<table->len;i++)
	{
		flag=0;

		if(cmd->cmd_args.sel_args.arg[0]==1)
		{
			if(cmd->cmd_args.sel_args.oper[0]==1)
			{
				if(table->users[i].id==cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==2)
			{
				if(table->users[i].id!=cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==3)
			{
				if(table->users[i].id>cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==4)
			{
				if(table->users[i].id<cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==5)
			{
				if(table->users[i].id>=cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==6)
			{
				if(table->users[i].id<=cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
		}
		else if(cmd->cmd_args.sel_args.arg[0]==2)
		{
			if(cmd->cmd_args.sel_args.oper[0]==1)
			{
				if(!strcmp(table->users[i].name,cmd->cmd_args.sel_args.data1))
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==2)
			{
				if(strcmp(table->users[i].name,cmd->cmd_args.sel_args.data1)!=0)
					flag=1;
			}
		}
		else if(cmd->cmd_args.sel_args.arg[0]==3)
		{
			if(cmd->cmd_args.sel_args.oper[0]==1)
			{
				if(!strcmp(table->users[i].email,cmd->cmd_args.sel_args.data1))
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==2)
			{
				if(strcmp(table->users[i].email,cmd->cmd_args.sel_args.data1)!=0)
					flag=1;
			}
		}
		else if(cmd->cmd_args.sel_args.arg[0]==4)
		{
			if(cmd->cmd_args.sel_args.oper[0]==1)
			{
				if(table->users[i].age==cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==2)
			{
				if(table->users[i].age!=cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==3)
			{
				if(table->users[i].age>cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==4)
			{
				if(table->users[i].age<cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==5)
			{
				if(table->users[i].age>=cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
			else if(cmd->cmd_args.sel_args.oper[0]==6)
			{
				if(table->users[i].age<=cmd->cmd_args.sel_args.data[0])
					flag=1;
			}
		}

		if(cmd->cmd_args.sel_args.logic==1)	//and
		{
			tmp=flag;

			if(cmd->cmd_args.sel_args.arg[1]==1)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(table->users[i].id==cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(table->users[i].id!=cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==3)
				{
					if(table->users[i].id>cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==4)
				{
					if(table->users[i].id<cmd->cmd_args.sel_args.data[1]  && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==5)
				{
					if(table->users[i].id>=cmd->cmd_args.sel_args.data[1]  && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==6)
				{
					if(table->users[i].id<=cmd->cmd_args.sel_args.data[1]  && tmp==1)
						flag=1;
					else
						flag=0;
				}
			}
			else if(cmd->cmd_args.sel_args.arg[1]==2)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(!strcmp(table->users[i].name,cmd->cmd_args.sel_args.data2)  && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(strcmp(table->users[i].name,cmd->cmd_args.sel_args.data2)!=0  && tmp==1)
						flag=1;
					else
						flag=0;
				}
			}
			else if(cmd->cmd_args.sel_args.arg[1]==3)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(!strcmp(table->users[i].email,cmd->cmd_args.sel_args.data2) && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(strcmp(table->users[i].email,cmd->cmd_args.sel_args.data2)!=0 && tmp==1)
						flag=1;
					else
						flag=0;
				}
			}
			else if(cmd->cmd_args.sel_args.arg[1]==4)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(table->users[i].age==cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(table->users[i].age!=cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==3)
				{
					if(table->users[i].age>cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==4)
				{
					if(table->users[i].age<cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==5)
				{
					if(table->users[i].age>=cmd->cmd_args.sel_args.data[1] && tmp==1)
						flag=1;
					else
						flag=0;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==6)
				{
					if(table->users[i].age<=cmd->cmd_args.sel_args.data[1]  && tmp==1)
						flag=1;
					else
						flag=0;
				}
			}
		}
		else if(cmd->cmd_args.sel_args.logic==0)	//or
		{
			if(cmd->cmd_args.sel_args.arg[1]==1)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(table->users[i].id==cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(table->users[i].id!=cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==3)
				{
					if(table->users[i].id>cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==4)
				{
					if(table->users[i].id<cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==5)
				{
					if(table->users[i].id>=cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==6)
				{
					if(table->users[i].id<=cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
			}
			else if(cmd->cmd_args.sel_args.arg[1]==2)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(!strcmp(table->users[i].name,cmd->cmd_args.sel_args.data2))
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(strcmp(table->users[i].name,cmd->cmd_args.sel_args.data2)!=0)
						flag=1;
				}
			}
			else if(cmd->cmd_args.sel_args.arg[1]==3)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(!strcmp(table->users[i].email,cmd->cmd_args.sel_args.data2))
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(strcmp(table->users[i].email,cmd->cmd_args.sel_args.data2)!=0)
						flag=1;
				}
			}
			else if(cmd->cmd_args.sel_args.arg[1]==4)
			{
				if(cmd->cmd_args.sel_args.oper[1]==1)
				{
					if(table->users[i].age==cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==2)
				{
					if(table->users[i].age!=cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==3)
				{
					if(table->users[i].age>cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==4)
				{
					if(table->users[i].age<cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==5)
				{
					if(table->users[i].age>=cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
				else if(cmd->cmd_args.sel_args.oper[1]==6)
				{
					if(table->users[i].age<=cmd->cmd_args.sel_args.data[1])
						flag=1;
				}
			}
		}

		if(flag)
		{
			*(where+pos)=i;
			pos++;
		}
	}

	if(cmd->cmd_args.sel_args.arrg==-1)
	{
		for (idx = offset; idx < pos; idx++)
		{
			if (limit != -1 && (idx - offset) >= limit)
			{
				break;
	            	}
	            	print_user(get_User(table, *(where+idx)), &(cmd->cmd_args.sel_args));
       		}
	}
	else
	{
		if(!offset && limit==-1)
		{
			    size_t idx;
			    int i,sum;
			    double avg;
			    int where_idx;
			
			    printf("(");
			    for (idx = 0; idx < cmd->cmd_args.sel_args.field_len; idx++)
			    {
				sum=0;
				avg=0;
				if (idx > 0) printf(", ");
			
				if (!strncmp(cmd->cmd_args.sel_args.field[idx], "avg", 3)) 
				{
					if(!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "id", 2))
					{ 
						for(i = 0;i<pos;i++)
						{
							where_idx=*(where+i);
							sum+=table->users[where_idx].id;
						}
						avg=(double)sum/(double)pos;
			                	printf("%.3lf", avg);
			            	}
					else if (!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "age", 3))
					{ 
						for(i = 0;i<pos;i++)
						{
							where_idx=*(where+i);
							sum+=table->users[where_idx].age;
						}
						avg=(double)sum/(double)pos;
			                	printf("%.3lf", avg);
			            	}
			        }
			        else if (!strncmp(cmd->cmd_args.sel_args.field[idx], "count", 5))
				{
			            printf("%d", pos);
			        }
				else if (!strncmp(cmd->cmd_args.sel_args.field[idx], "sum", 3)) 
				{
					if(!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "id", 2))
					{ 
						for(i = 0;i<pos;i++)
						{
							where_idx=*(where+i);
							sum+=table->users[where_idx].id;
						}
			                	printf("%d", sum);
			            	}
					else if (!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "age", 3))
					{ 
						for(i = 0;i<pos;i++)
						{
							where_idx=*(where+i);
							sum+=table->users[where_idx].age;
						}
			                	printf("%d", sum);
			            	}
			        }
			    }
    			    printf(")\n");
		}
	}
    }
    else {
	if(cmd->cmd_args.sel_args.arrg==-1)
	{
	        for (idx = offset; idx < table->len; idx++) {
	            if (limit != -1 && (idx - offset) >= limit) {
	                break;
	            }
	            print_user(get_User(table, idx), &(cmd->cmd_args.sel_args));
		}
        }
	else
	{
		if(!offset && limit==-1)
		{
			    size_t idx;
			    int i,sum;
			    double avg;
			
			    printf("(");
			    for (idx = 0; idx < cmd->cmd_args.sel_args.field_len; idx++)
			    {
				sum=0;
				avg=0;
				if (idx > 0) printf(", ");
			
				if (!strncmp(cmd->cmd_args.sel_args.field[idx], "avg", 3)) 
				{
					if(!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "id", 2))
					{ 
						for(i = 0;i<table->len;i++)
						{
							sum+=table->users[i].id;
						}
						avg=(double)sum/(double)table->len;
			                	printf("%.3lf", avg);
			            	}
					else if (!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "age", 3))
					{ 
						for(i = 0;i<table->len;i++)
						{
							sum+=table->users[i].age;
						}
						avg=(double)sum/(double)table->len;
			                	printf("%.3lf", avg);
			            	}
			        }
			        else if (!strncmp(cmd->cmd_args.sel_args.field[idx], "count", 5))
				{
			            printf("%ld", table->len);
			        }
				else if (!strncmp(cmd->cmd_args.sel_args.field[idx], "sum", 3)) 
				{
					if(!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "id", 2))
					{ 
						for(i = 0;i<table->len;i++)
						{
							sum+=table->users[i].id;
						}
			                	printf("%d", sum);
			            	}
					else if (!strncmp(cmd->cmd_args.sel_args.field[idx]+4, "age", 3))
					{ 
						for(i = 0;i<table->len;i++)
						{
							sum+=table->users[i].age;
						}
			                	printf("%d", sum);
			            	}
			        }
			    }
    			    printf(")\n");
		}
	}
    }
}

///
/// This function received an output argument
/// Return: category of the command
///
int parse_input(char *input, Command_t *cmd) {
    char *token;
    int idx;
    token = strtok(input, " ,\n");
    for (idx = 0; strlen(cmd_list[idx].name) != 0; idx++) {
        if (!strncmp(token, cmd_list[idx].name, cmd_list[idx].len)) {
            cmd->type = cmd_list[idx].type;
        }
    }
    while (token != NULL) {
        add_Arg(cmd, token);
        token = strtok(NULL, " ,\n");
    }
    return cmd->type;
}

///
/// Handle built-in commands
/// Return: command type
///
void handle_builtin_cmd(Table_t *table, Command_t *cmd, State_t *state) {
    if (!strncmp(cmd->args[0], ".exit", 5)) {
        archive_table(table);
        exit(0);
    } else if (!strncmp(cmd->args[0], ".output", 7)) {
        if (cmd->args_len == 2) {
            if (!strncmp(cmd->args[1], "stdout", 6)) {
                close(1);
                dup2(state->saved_stdout, 1);
                state->saved_stdout = -1;
            } else if (state->saved_stdout == -1) {
                int fd = creat(cmd->args[1], 0644);
                state->saved_stdout = dup(1);
                if (dup2(fd, 1) == -1) {
                    state->saved_stdout = -1;
                }
                __fpurge(stdout); //This is used to clear the stdout buffer
            }
        }
    } else if (!strncmp(cmd->args[0], ".load", 5)) {
        if (cmd->args_len == 2) {
            load_table(table, cmd->args[1]);
        }
    } else if (!strncmp(cmd->args[0], ".help", 5)) {
        print_help_msg();
    }
}

///
/// Handle query type commands
/// Return: command type
///
int handle_query_cmd(Table_t *table, Command_t *cmd) {
    if (!strncmp(cmd->args[0], "insert", 6)) {
        handle_insert_cmd(table, cmd);
        return INSERT_CMD;
    } else if (!strncmp(cmd->args[0], "select", 6)) {
        handle_select_cmd(table, cmd);
        return SELECT_CMD;
    } else {
        return UNRECOG_CMD;
    }
}

///
/// The return value is the number of rows insert into table
/// If the insert operation success, then change the input arg
/// `cmd->type` to INSERT_CMD
///
int handle_insert_cmd(Table_t *table, Command_t *cmd) {
    int ret = 0;
    User_t *user = command_to_User(cmd);
    if (user) {
        ret = add_User(table, user);
        if (ret > 0) {
            cmd->type = INSERT_CMD;
        }
    }
    return ret;
}

///
/// The return value is the number of rows select from table
/// If the select operation success, then change the input arg
/// `cmd->type` to SELECT_CMD
///
int handle_select_cmd(Table_t *table, Command_t *cmd) {
    cmd->type = SELECT_CMD;
    field_state_handler(cmd, 1);

    print_users(table, NULL, 0, cmd);
    return table->len;
}

///
/// Show the help messages
///
void print_help_msg() {
    const char msg[] = "# Supported Commands\n"
    "\n"
    "## Built-in Commands\n"
    "\n"
    "  * .exit\n"
    "\tThis cmd archives the table, if the db file is specified, then exit.\n"
    "\n"
    "  * .output\n"
    "\tThis cmd change the output strategy, default is stdout.\n"
    "\n"
    "\tUsage:\n"
    "\t    .output (<file>|stdout)\n\n"
    "\tThe results will be redirected to <file> if specified, otherwise they will display to stdout.\n"
    "\n"
    "  * .load\n"
    "\tThis command loads records stored in <DB file>.\n"
    "\n"
    "\t*** Warning: This command will overwrite the records already stored in current table. ***\n"
    "\n"
    "\tUsage:\n"
    "\t    .load <DB file>\n\n"
    "\n"
    "  * .help\n"
    "\tThis cmd displays the help messages.\n"
    "\n"
    "## Query Commands\n"
    "\n"
    "  * insert\n"
    "\tThis cmd inserts one user record into table.\n"
    "\n"
    "\tUsage:\n"
    "\t    insert <id> <name> <email> <age>\n"
    "\n"
    "\t** Notice: The <name> & <email> are string without any whitespace character, and maximum length of them is 255. **\n"
    "\n"
    "  * select\n"
    "\tThis cmd will display all user records in the table.\n"
    "\n";
    printf("%s", msg);
}

