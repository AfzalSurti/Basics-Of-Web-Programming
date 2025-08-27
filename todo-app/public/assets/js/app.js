async function fetchTasks() {
  const response = await fetch(window.API_BASE, { method: 'GET' });
  const data = await response.json();
  if (!response.ok) throw new Error(data.error || 'Failed to fetch tasks');
  return data.tasks || [];
}

async function createTask(title) {
  const response = await fetch(window.API_BASE, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ title })
  });
  const data = await response.json();
  if (!response.ok) throw new Error(data.error || 'Failed to create task');
  return data.task;
}

async function updateTask(update) {
  const response = await fetch(window.API_BASE, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(update)
  });
  const data = await response.json();
  if (!response.ok) throw new Error(data.error || 'Failed to update task');
  return data.task;
}

async function deleteTask(id) {
  const response = await fetch(`${window.API_BASE}?id=${encodeURIComponent(id)}`, {
    method: 'DELETE'
  });
  const data = await response.json();
  if (!response.ok) throw new Error(data.error || 'Failed to delete task');
  return true;
}

function showAlert(message, type = 'success') {
  const alerts = document.getElementById('alerts');
  alerts.innerHTML = `<div class="alert alert-${type} alert-dismissible fade show" role="alert">${message}<button type="button" class="btn-close" data-bs-dismiss="alert" aria-label="Close"></button></div>`;
}

function renderTasks(tasks) {
  const list = document.getElementById('task-list');
  list.innerHTML = '';
  tasks.forEach(task => {
    const item = document.createElement('li');
    item.className = `list-group-item d-flex align-items-center justify-content-between ${task.is_completed ? 'completed' : ''}`;
    item.dataset.id = task.id;

    const left = document.createElement('div');
    left.className = 'form-check';
    left.innerHTML = `
      <input class="form-check-input" type="checkbox" ${task.is_completed ? 'checked' : ''}>
      <label class="form-check-label task-title">${task.title}</label>
    `;

    const actions = document.createElement('div');
    actions.className = 'task-actions';
    actions.innerHTML = `
      <button class="btn btn-sm btn-outline-secondary edit-btn">Edit</button>
      <button class="btn btn-sm btn-outline-danger delete-btn">Delete</button>
    `;

    item.appendChild(left);
    item.appendChild(actions);
    list.appendChild(item);
  });
}

async function reload() {
  try {
    const tasks = await fetchTasks();
    renderTasks(tasks);
  } catch (err) {
    showAlert(err.message, 'danger');
  }
}

document.addEventListener('DOMContentLoaded', () => {
  const form = document.getElementById('task-form');
  const titleInput = document.getElementById('task-title');

  form.addEventListener('submit', async (e) => {
    e.preventDefault();
    const title = titleInput.value.trim();
    if (!title) return;
    try {
      await createTask(title);
      titleInput.value = '';
      await reload();
      showAlert('Task added');
    } catch (err) {
      showAlert(err.message, 'danger');
    }
  });

  document.getElementById('task-list').addEventListener('click', async (e) => {
    const item = e.target.closest('li.list-group-item');
    if (!item) return;
    const id = Number(item.dataset.id);

    if (e.target.matches('input.form-check-input')) {
      const checked = e.target.checked;
      try {
        await updateTask({ id, is_completed: checked });
        await reload();
      } catch (err) { showAlert(err.message, 'danger'); }
    }

    if (e.target.matches('.delete-btn')) {
      try {
        await deleteTask(id);
        await reload();
        showAlert('Task deleted');
      } catch (err) { showAlert(err.message, 'danger'); }
    }

    if (e.target.matches('.edit-btn')) {
      const titleEl = item.querySelector('.task-title');
      const current = titleEl.textContent;
      const next = prompt('Edit task title:', current);
      if (next === null) return;
      const newTitle = next.trim();
      if (!newTitle || newTitle === current) return;
      try {
        await updateTask({ id, title: newTitle });
        await reload();
        showAlert('Task updated');
      } catch (err) { showAlert(err.message, 'danger'); }
    }
  });

  reload();
});

