## To-Do List App (Bootstrap + PHP + MySQL)

This is a simple full-stack to-do list application with:
- Frontend: HTML, CSS, Bootstrap 5, JavaScript (Fetch API)
- Backend: PHP (PDO)
- Database: MySQL (XAMPP compatible; import via phpMyAdmin)

### Features
- Add a task
- Toggle task as completed
- Delete a task
- Responsive UI with Bootstrap

### Project Structure
```
todo-app/
  public/
    index.html
    assets/
      css/styles.css
      js/app.js
  api/
    config.php
    tasks.php
  database/
    schema.sql
  README.md
```

### Prerequisites
- XAMPP installed
- PHP 8+ recommended
- MySQL running

### Setup
1) Create the database and table
   - Open phpMyAdmin at `http://localhost/phpmyadmin`
   - Import the file `database/schema.sql`

2) Configure PHP connection (if needed)
   - Open `api/config.php`
   - Default credentials (typical for XAMPP):
     - host: 127.0.0.1
     - db: todo_app
     - user: root
     - pass: "" (empty)
   - Adjust if your MySQL setup differs.

3) Place the project in XAMPP htdocs
   - Copy the `todo-app` folder into your XAMPP `htdocs` directory
     - Example on Windows: `C:\xampp\htdocs\todo-app`
     - Example on Linux: `/opt/lampp/htdocs/todo-app`

4) Open the app
   - Visit `http://localhost/todo-app/public/`

### API Endpoints
- `GET    /api/tasks.php`           → List tasks
- `POST   /api/tasks.php`           → Create task `{ title }`
- `PUT    /api/tasks.php`           → Update task `{ id, title?, is_completed? }`
- `DELETE /api/tasks.php?id=123`    → Delete task by id (or send `{ id }` JSON)

All requests and responses use JSON. For `PUT`/`DELETE`, you can also provide `id` via query string.

### Notes
- CORS headers are permissive for local development. For production, restrict `Access-Control-Allow-Origin` and methods as needed.
- Error details are verbose in development.

